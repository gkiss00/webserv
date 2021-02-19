#ifndef MYWEBSERVER_HPP
#define MYWEBSERVER_HPP

#include "webserv.hpp"

extern int	g_thread_pool_size;

class Client;

class MyWebServer {
    public:

#ifdef __APPLE__
        MyWebServer(std::string config_path = "srcs/config/macos.conf");
#else
        MyWebServer(std::string config_path = "srcs/config/linux.conf");
#endif
        ~MyWebServer();

        // Sockets
        void error_socket (const char * error_msg, int sock);
        int new_socket (const char *host, int port);
        void _bind_all ();

        // Connect
        int accept_client(int server_sock);
        void remove_client(int client_sock);

        int _send(int sock, std::string msg);
        std::string _recv(int sock, bool &is_good);

        Server &server_from_fd(int fd);

        void run();

        bool handle_request(int fd);

        std::map<int, Client>  clients; // client_sock : Client
        std::vector<int>    server_sockets;
        std::vector<Server> servers;
};

#endif