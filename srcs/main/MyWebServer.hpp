#ifndef MYWEBSERVER_HPP
#define MYWEBSERVER_HPP

#include "webserv.hpp"

class MyWebServer {
    public:

        class BindFailed : public std::exception {
            public:
                virtual const char * what() const throw();
        };

        MyWebServer(std::string config_path = "srcs/config/new.conf");
        ~MyWebServer();

        // Sockets
        void close_socket (const char * error_msg, int sock);
        int new_socket (const char *host, int port);
        void _bind_all (const char *host = "127.0.0.1");

        // Connect
        int accept_client(int server_sock);

        int _send(int sock, std::string msg);
        std::string _recv(int sock);

        Server &server_from_fd(int fd);

        void run();

    private:
        fd_set              current_sockets; // list of all sockets.
        std::map<int, int>  client_server; // client_sock : server_sock
        std::queue<int>     queue_clients;
        std::vector<int>    server_sockets;
        std::vector<Server> servers;
};

#endif