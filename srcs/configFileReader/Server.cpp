#include "Server.hpp"

Server::Server(){
    this->socket = -1;
    this->listen = -1;
    this->client_max_body_size = -1;
    this->autoindex = false;
    this->root = "";
    this->default_file = "";
}

Server::~Server(){}

void    Server::print(){
    std::cout << "socket : " << socket << std::endl;
    std::cout << "listen : " << listen << std::endl;
    std::cout << "server_names : ";
    for (unsigned int i = 0; i < server_names.size(); ++i){
        std::cout << server_names.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "error_pages : ";
    for (std::map<int,std::string>::iterator it=error_pages.begin(); it!=error_pages.end(); ++it){
        std::cout << it->first << " => " << it->second << " ";
    }
    std::cout << std::endl;
    std::cout << "max_body : " << client_max_body_size << std::endl;
    std::cout << "methodes : ";
    for (unsigned int i = 0; i < methodes.size(); ++i){
        std::cout << methodes.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "root : " << root << std::endl;
    std::cout << "autoindex : " << autoindex << std::endl;
    std::cout << "default_file : " << default_file << std::endl;
    std::cout << "metavariables : ";
    for (unsigned int i = 0; i < metavariables.size(); ++i){
        std::cout << metavariables.at(i) << " ";
    }
    std::cout << std::endl;

}