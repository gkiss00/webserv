#include "Server.hpp"

Server::Server(){
    this->socket = -1;
    this->listen = -1;
    this->client_max_body_size = -1;
}

Server::~Server(){}

void    Server::print(){
#ifdef DEBUG
    std::cout << "SOCKET : " << socket << std::endl;
    std::cout << "PORT : " << listen << std::endl;
    std::cout << "SERVER_NAMES : ";
    for (unsigned int i = 0; i < server_names.size(); ++i){
        std::cout << server_names.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "ERROR_PAGES : ";
    for (std::map<int,std::string>::iterator it=error_pages.begin(); it!=error_pages.end(); ++it){
        std::cout << it->first << " => " << it->second << " ";
    }
    std::cout << std::endl;
    std::cout << "MAX_BODY : " << client_max_body_size << std::endl;
    std::cout << std::endl;
    std::cout << "METAVAR : ";
    for (unsigned int i = 0; i < metavariables.size(); ++i){
        std::cout << metavariables.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "LOCATIONS : ";
    for (unsigned int i = 0; i < locations.size(); ++i){
        locations.at(i).print();
    }
    std::cout << std::endl;
#endif
}