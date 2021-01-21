#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

#include "webserv.hpp"

#define GET 0

class RequestParser
{
    public:
    RequestParser(std::string request);
    ~RequestParser();

    std::string command;
    std::string path;
    double      HTTP_version;
    std::map<std::string, std::string>  headers;
    std::string body;

    private:
    void        parse(std::string request);
    std::string get_next(std::string &str, const std::string delimiter);
};

#endif