#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"

class Location;

class Server {
    public:
        Server();
        ~Server();

        int socket;
        int listen;
        std::vector<std::string> server_names;
        std::map<int, std::string> error_pages;
        int client_max_body_size;
        // std::vector<std::string> methodes;
        // std::string root;
        // bool autoindex;
        // std::string default_file;
        std::vector<std::string> metavariables;
        std::vector<Location> locations;
        std::vector<int> client_sockets;

        void print();
};

#endif