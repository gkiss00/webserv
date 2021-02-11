#include "MyWebServer.hpp"
#define MAX_CLIENT


// Based on https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
// https://www.tenouk.com/Module41a.html

MyWebServer::MyWebServer(std::string config_path) {
    NewConfigFileReader cfr;
    this->servers = cfr.read(config_path);
    this->_bind_all();
}

MyWebServer::~MyWebServer() {

    // Close the servers
    for (size_t i = 0; i < servers.size(); ++i)
        close(servers[i].socket);

    // Close the clients
    while (queue_clients.size()) {
        close(queue_clients.front());
        clients.erase(queue_clients.front());
        queue_clients.pop_front();
    }
}

// SOCKET + BIND
void    MyWebServer::error_socket (const char *error_msg, int sock) {
    close (sock);
    std::cout << "\e[31m" << error_msg << " failed\e[m" << std::endl;
}

int    MyWebServer::new_socket(const char *host, int port) {
    int                 sock;
    struct sockaddr_in  addrServer;

    //fill the struct the server socket will refere to
    addrServer.sin_addr.s_addr = inet_addr(host);     //HOST
    addrServer.sin_family = AF_INET;                  //IPV4
    addrServer.sin_port = ft_htons(port);             //PORT

    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        error_socket ("socket", -1);

    int option = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int)) == -1)
        error_socket ("setsockopt SO_REUSEADDR", sock);

    //bind the server socket with the struct
    if (bind(sock, (const struct sockaddr *)&(addrServer), sizeof(addrServer)) == -1)
        error_socket ("bind", sock);
    std::cout << "Bind #" << sock << ", port #" << port << std::endl;

    // NONBLOCK
    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
        error_socket ("Non blocking", sock);

    // Listen
    if (listen(sock, 1000) == -1)
        error_socket ("Listen", sock); // 1000 queue accepted
    
    this->server_sockets.push_back(sock);
    return sock;
}

void    MyWebServer::_bind_all () {
    for (unsigned int i = 0; i < servers.size(); ++i) {
        std::cout << servers.at(i).host << std::endl;
        servers.at(i).socket = this->new_socket(servers.at(i).host.c_str(), servers.at(i).listen);
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
    char        buf[100001];
    int         ret;

    ret = recv(sock, buf, 100000, 0); 
    buf[ret] = '\0';
    if (ret == -1)
        std::cerr << "recv fail" << std::endl; // must close
    return std::string(buf);
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

    // Add client to queue / map
    clients[client_sock] = Client(server_sock);
    queue_clients.push_back(client_sock);

    // suppress first client if too much of them
    if (this->queue_clients.size() > 200) // limit the amount of clients
        this->remove_client(queue_clients.front());

    return client_sock;
}

void     MyWebServer::remove_client(int client_sock) {
    close(client_sock);
    queue_clients.remove(client_sock);
    clients.erase(client_sock);
}


Server &MyWebServer::server_from_fd(int fd) {
    for (size_t i = 0; i < servers.size(); ++i) {
        if (servers[i].socket == fd) return servers[i];
    }
    return servers[0];
}


// RUN
void    MyWebServer::run() {
    while (true) {

        fd_set  reading_sockets, writing_sockets;

        // set reading_sockets for select
        int max = 0;
        FD_ZERO(&reading_sockets);
        FD_ZERO(&writing_sockets);
        for (size_t i = 0; i < servers.size(); ++i)
        {
            FD_SET(servers[i].socket, &reading_sockets);
            FD_SET(servers[i].socket, &writing_sockets);
            if (servers[i].socket > max) max = servers[i].socket;
        }
        for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            // std::cout << "[] = " << it->first << std::endl;
            FD_SET(it->first, &reading_sockets);
            FD_SET(it->first, &writing_sockets);
            if (it->first > max) max = it->first;
        }
        //std::cout << "before select" << std::endl;
        // SELECT
        if (select(max + 1, &reading_sockets, &writing_sockets, NULL, NULL) == -1) {
            std::cout << "select failed" << std::endl;
            perror("");
            break ;
        }
        //std::cout << "after select" << std::endl;
        // Go through fds
        for (int fd = 0; fd <= max; ++fd) {


            // If the fd is in the set
            if (FD_ISSET(fd, &reading_sockets)) {

                // ADD THE CLIENT
                if (std::count(this->server_sockets.begin(), this->server_sockets.end(), fd)) {
                    //std::cout << "before accept" << std::endl;
                    accept_client(fd);
                    //std::cout << "after accept" << std::endl;
                }
                else {
                    clients[fd].add_content(_recv(fd));
                    // BLU(clients[fd].is_ready());
                    // GRN(clients[fd].get_content());
                    if (clients[fd].is_ready()) {
                        try {
                            //std::cout << "before get" << std::endl;
                            RequestParser   request(clients[fd].get_content());
                            //std::cout << "middle get" << std::endl;
                            Response response(request, server_from_fd(clients[fd].server_sd));
                            //std::cout << "middle 2 get" << std::endl;
                            // client[fd].clear_response();
                            clients[fd].add_response(response.render(), request.command == "PUT");
                            //std::cout << "after accept" << std::endl;
                        } catch(request_exception &e) {
                            std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
                            _send(fd, "HTTP/1.1 " + std::to_string(e.get_error_status()) + " " + statusCodes()[e.get_error_status()] + "\n");
                        }
                        clients[fd].clear_content();
                    }
                }
            }
            if (FD_ISSET(fd, &writing_sockets)) {

                std::string mail(clients[fd].get_response());

                if (mail != "") {

                    _send(fd, mail);
                    clients[fd].clear_response();
                    
                    if (clients[fd].get_is_put()) {
                        remove_client(fd);
                        break ;
                    }
                }
            }
        }
    }
}