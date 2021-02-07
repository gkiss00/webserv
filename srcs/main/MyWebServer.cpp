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
    addrServer.sin_family = AF_INET;                  //IPV4
    addrServer.sin_port = htons(port);                //PORT

    // if ((sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    if ((sock = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        perror ("socket");

    int option = 1;
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
<<<<<<< HEAD
            // std::cout << "bad bad bad" << std::endl;
=======
            std::cout << "bad bad bad" << std::endl;
            usleep(1000);
>>>>>>> 460c8f8f378db10c525d4bbd5e3d22956069402e
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
        if ((request.find("\r\n\r\n") != std::string::npos && request.find("chunked") == std::string::npos) || // we find that it is not chunked
            request.find("0\r\n\r\n") != std::string::npos) {
           break ;
        }
        usleep(100);
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
    if (fcntl(client_sock, F_GETFL, O_NONBLOCK) == -1)
        close_socket ("Non blocking", client_sock);

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
<<<<<<< HEAD
        struct timeval tv = {2, 0};
        if (select(max + 1, &current_sockets, NULL, NULL, &tv) == -1) {
=======
        
        if (select(max + 1, &current_sockets, NULL, NULL, NULL) == -1) {
>>>>>>> 460c8f8f378db10c525d4bbd5e3d22956069402e
            std::cout << "select failed" << std::endl;
            continue ;
        }

        // Check for internal
        for (int fd = 0; fd <= max; ++fd) {

            //if the fd is in the set
            if (FD_ISSET(fd, &current_sockets)) {
                
                // ADD THE CLIENT
                if (std::count(this->server_sockets.begin(), this->server_sockets.end(), fd)) {
                    int client_sock = accept_client(fd);
                    client_server[client_sock] = fd;
                    if (client_sock > max) max = client_sock;
                    // FD_SET(client_sock, &current_sockets);
                    std::cout << "client #" << client_sock << " connected to #" << fd << std::endl;
                    usleep(10000);
                }

                // HANDLE THE CLIENT
                
                else {
                    try {
<<<<<<< HEAD
                        std::string content = _recv(fd);
                        if (content != "")
                        {
                            RequestParser   request(content);
                            Response response(request, server_from_fd(client_server[fd]));
                            std::string response_render(response.render());
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
=======
                        
                        RequestParser   request(_recv(fd));
                        Response response(request, server_from_fd(client_server[fd]));
                        
                        _send(fd, response.render());
                        FD_CLR(fd, &current_sockets);
>>>>>>> 460c8f8f378db10c525d4bbd5e3d22956069402e
                    } catch(request_exception &e) {
                        // bad message
                        std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
                        _send(fd, "HTTP/1.1 " + std::to_string(e.get_error_status()) + " " + statusCodes()[e.get_error_status()] + "\n");
                    }
                }
            }
        }

        // // HARD RESET WHEN NO CLIENT
        // if (client_server.size() == 0)
        // {
        //     std::cout << "HARD RESET" << std::endl;
        //     for (size_t i = 0; i < server_sockets.size(); ++i) {
        //         if (close(server_sockets[i]) < 0) perror("close main connection");
        //         FD_CLR(server_sockets[i], &current_sockets);
                
        //     }
        //     server_sockets.clear();
        //     _bind_all();
        // }
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
