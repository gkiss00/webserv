#include "webserv.hpp"

std::string trim(std::string str){
    int     start;
    int     end;
    
    start = str.find_first_not_of(" ");
    end = str.find_last_not_of(" ");
    str.erase(end + 1);
    str.erase(0, start);
    return (str);
}

std::string trim(std::string str, std::string tr){
    int     start;
    int     end;
    
    start = str.find_first_not_of(tr);
    end = str.find_last_not_of(tr);
    str.erase(end + 1);
    str.erase(0, start);
    return (str);
}