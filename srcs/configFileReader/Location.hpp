#ifndef LOCATION_HPP
#define LOCATION_HPP

# include "webserv.hpp"

class Location {
    public:
        Location();
        ~Location();

        std::string dir;                    //the dir
        std::vector<std::string> methods;   //list of accepted methods
        std::string root;                   //root of the dir
        bool autoindex;                     //is autoindex activated
        std::string default_file;           //default file if a dir is in the url
        std::map<std::string, std::string> cgi;    //a map of all accepted CGI and their path
        std::string upload;                 //the path to upkload files with PUT request
        bool regexx;                         //does the rout works with regex
        bool proxy;                         //proxy server

        void print();
};

#endif