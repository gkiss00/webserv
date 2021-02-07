#include "MyWebServer.hpp"

// Based on https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
// https://www.tenouk.com/Module41a.html

MyWebServer::MyWebServer(std::string config_path) {
    NewConfigFileReader cfr;
    this->servers = cfr.read(config_path);
    this->_bind_all("127.0.0.1");
}

MyWebServer::~MyWebServer() {}

// SOCKET + BIND
void    MyWebServer::close_socket (const char *error_msg, int sock) {
    close (sock);
    perror (error_msg);
}

int    MyWebServer::new_socket(const char *host, int port) {
    int                 sock;
    struct sockaddr_in  addrServer;

    bzero(&addrServer, sizeof(addrServer));
    //fill the struct the server socket will refere to
    addrServer.sin_addr.s_addr = inet_addr(host);     //HOST
    addrServer.sin_family = PF_INET;                  //IPV4
    addrServer.sin_port = htons(port);                //PORT

    // if ((sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    if ((sock = socket (PF_INET, SOCK_STREAM, 0)) == -1)
        perror ("socket");

    int option = 1;
    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &option, sizeof(int)) == -1)
        close_socket ("setsockopt SO_REUSEADDR", sock);
    option = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int)) == -1)
        close_socket ("setsockopt SO_REUSEADDR", sock);
    // int size_buf = 1000;
    // if (setsockopt (sock, SOL_SOCKET, SO_SNDBUF, &size_buf, sizeof (int)) == -1)
    //     close_socket ("setsockopt SO_SNDBUF", sock);

    //bind the server socket with the struct
    if (bind(sock, (const struct sockaddr *)&(addrServer), sizeof(addrServer)) == -1)
        close_socket ("bind", sock);
    std::cout << "Bind #" << sock << ", port #" << port << std::endl;
    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
        close_socket ("Non blocking", sock);
    // if (fcntl(sock, F_GETFL, O_NONBLOCK) == -1)
    //     close_socket ("Non blocking", sock);

    listen(sock, 1000);
    this->server_sockets.push_back(sock);
    return sock;
}

void    MyWebServer::_bind_all (const char *host) {
    for (unsigned int i = 0; i < servers.size(); ++i) {
        servers.at(i).socket = this->new_socket(host, servers.at(i).listen);
        if(servers.at(i).socket == -1) {
            std::cout << "Error while init server socket" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// SEND
int     MyWebServer::_send(int sock, std::string msg) {
    int tot = 0;
    while(msg != "") {
        // std::cout << "GOOD OR BAD?" << std::endl;
        int ret = send(sock, msg.c_str(), msg.size(), 0);
        if (ret < 0) {
            // std::cout << "bad bad bad" << std::endl;
        } else if (ret != (int)msg.size()) {
            msg = msg.substr(ret);
            tot += ret;
            // std::cout << "tot= " << tot << std::endl;
        } else {
            // std::cout << "good good good " << std::endl;
            break;
        }
    }
    return tot;
}

// RECV
std::string    MyWebServer::_recv(int sock) {
    std::string request;
    char        buf[100001];
    int         ret;

    while ((ret = recv(sock, buf, 100000, 0)) > 0) {
        buf[ret] = '\0';
        request += buf;
        if ((request.find("chunked") == std::string::npos && request.find("\r\n\r\n") != std::string::npos) || // we find that it is not chunked
            request.find("0\r\n\r\n") != std::string::npos) {
           break ;
        }
    }
    if (ret == -1) perror("recv");
    return request;
}

// ACCEPT CLIENT
int     MyWebServer::accept_client(int server_sock) {
    int                 client_sock;
    struct sockaddr     client_ad;
    socklen_t           client_len = sizeof(client_ad);

    // Wait for a client to connect
    if ((client_sock = accept(server_sock, &client_ad, &client_len)) < 0)
        perror("accept");

    std::cout << "client #" << client_sock << " connected to #" << server_sock << std::endl;

    // add client to queue / map
    client_server[client_sock] = server_sock;
    queue_clients.push(client_sock);

    // suppress first client if too much of them
    if (this->queue_clients.size() > 200) // limit the amount of clients
    {
        close(queue_clients.front());
        client_server.erase(queue_clients.front());
        FD_CLR(queue_clients.front(), &current_sockets);
        queue_clients.pop();
    }

    return client_sock;
}

Server &MyWebServer::server_from_fd(int fd) {
    for (size_t i = 0; i < servers.size(); ++i)
    {
        if (servers[i].socket == fd) return servers[i];
    }
    return servers[0];
}


// RUN
void    MyWebServer::run() {
    while (true) {
        std::map<int, int>::iterator it;

        // set current_sockets for select
        int max = 0;
        FD_ZERO(&current_sockets);
        for (size_t i = 0; i < servers.size(); ++i)
        {
            FD_SET(servers[i].socket, &current_sockets);
            if (servers[i].socket > max) max = servers[i].socket;
        }
        for (it = client_server.begin(); it != client_server.end(); ++it)
        {
            FD_SET(it->first, &current_sockets);
            if (it->first > max) max = it->first;
        }

        // because select is destructive
        // struct timeval tv = {2, 0};
        if (select(max + 1, &current_sockets, NULL, NULL, NULL) == -1) {
            std::cout << "select failed" << std::endl;
            break ;
        }

        // Check for internal
        for (int fd = 0; fd <= max; ++fd) {

            //if the fd is in the set
            if (FD_ISSET(fd, &current_sockets)) {
                
                // ADD THE CLIENT
                if (std::count(this->server_sockets.begin(), this->server_sockets.end(), fd)) {
                    accept_client(fd);
                }

                // HANDLE THE CLIENT
                else {
                    try {
                        // clock_t time_req = clock(); // TIME_REQUIRED
                        
                        std::string content = _recv(fd);
                        if (content != "")
                        {
                            // BLU(clock() - time_req) // TIME TO RECEIVE BLUE

                            // time_req = clock(); // TIME_REQUIRED
                            RequestParser   request(content);
                            // YLW(clock() - time_req) // TIME TO PARSE YLW
                        
                            // time_req = clock(); // TIME_REQUIRED
                            Response response(request, server_from_fd(client_server[fd]));                            
                            std::string response_render(response.render());
                            // GRN(clock() - time_req) // TIME TO ANSWER GRN

                            // time_req = clock(); // TIME_REQUIRED

                            _send(fd, response_render);
                            FD_CLR(fd, &current_sockets);
                            if (request.command == "GET" && content.size() != 95) {
                                YLW(content.size())
                            }
                            if (request.command == "PUT"){
                                if (close(fd) < 0)
                                    perror("closing client");
                                client_server.erase(fd);
                            }
                        }
                    } catch(request_exception &e) {
                        // bad message
                        std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
                        _send(fd, "HTTP/1.1 " + std::to_string(e.get_error_status()) + " " + statusCodes()[e.get_error_status()] + "\n");
                    }
                }
            }
        }
    }
}



// std::cout << "__________client_request__________" << std::endl;
// std::cout << "Command = " << request.command << std::endl;
// std::cout << "Path = " << request.path << std::endl;
// std::cout << "HTTP_version = " << request.HTTP_version << std::endl;
// for (std::map<std::string, std::string>::iterator it = request.headers.begin(); it != request.headers.end(); ++it){
//     std::cout << "Headers = " << it->first << " : " << it->second << std::endl;
// }
// std::cout << "Body[" << request.body.size() << "] = " << request.body.substr(0, 1000) << std::endl;
// //write
