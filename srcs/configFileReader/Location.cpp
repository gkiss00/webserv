#include "Location.hpp"

Location::Location(){}

Location::~Location(){}

void Location::print(){
    std::cout << "__________LOCATION__________" << std::endl;
    std::cout << "DIR : " << this->dir << std::endl;
    std::cout << "METHODS : ";
    for (unsigned int i = 0; i < this->methods.size(); ++i){
        std::cout << this->methods.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "ROOT : " << this->root << std::endl;
    std::cout << "AUTO_INDEX : " << this->autoindex << std::endl;
    std::cout << "DEFAULT_FILE : " << this->default_file << std::endl;
    std::cout << "CGI : ";
    for(std::map<std::string, std::string>::iterator it = this->cgi.begin(); it != this->cgi.end(); ++it){
        std::cout << it->first << " : " << std::cout << it->second;
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "UPLOAD : " << this->upload << std::endl;
}