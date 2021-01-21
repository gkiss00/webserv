#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "webserv.hpp"

class RequestParser;

class Response
{
    public:
    Response(RequestParser &query);
    ~Response();

    void parse();

    void getStatus();
    void getFile();

    std::string render();

    private:
    RequestParser       &query;
    const std::string   error_pages;
    int                 status;
    std::string         content;
    std::string         content_type;
};

#endif