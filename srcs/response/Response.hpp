#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ResponseHeader.hpp"
# include "webserv.hpp"

class RequestParser;
class ResponseHeader;

class Response
{
    public:
    Response(RequestParser &query);
    ~Response();

    void parse();

    void getStatus();
    void getFile();
    void moveFile();

    std::string render();

    private:
    RequestParser       &query;
    ResponseHeader      header;
    const std::string   error_pages;
    int                 status;
    std::string         content;
};

#endif