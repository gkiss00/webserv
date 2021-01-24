#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ResponseHeader.hpp"
# include "webserv.hpp"

class Server;
class RequestParser;
class ResponseHeader;

class Response
{
    public:
    Response(RequestParser &query, Server &server);
    ~Response();

    void parse();

    void getStatus();
    void getFile();
    void moveFile();
    void execCGI();
    void setAllowedMethods();

    std::string render();

    private:
    RequestParser       &query;
    Server              &server;
    ResponseHeader      header;
    int                 status;
    std::string         content;
};

#endif