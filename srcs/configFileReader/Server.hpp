#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp";

class Server {
    public:
        Server();
        ~Server();
        int socket;
        int listen;
        std::vector<std::string> server_names;
        int client_max_body_size;
        std::vector<std::string> methodes;
        std::string root;
        bool autoindex;
        std::string default_file;
        std::vector<std::string> metavariables;
};

Server::Server(){
    this->socket = -1;
    this->listen = -1;
    this->client_max_body_size = -1;
    this->autoindex = false;
    this->root = "";
    this->default_file = "";
}

Server::~Server(){}

#endif