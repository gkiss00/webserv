#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ResponseHeader.hpp"
# include "PostHeader.hpp"
# include "webserv.hpp"

class Server;
class RequestParser;
class ResponseHeader;

class Response
{
    public:
    Response(RequestParser &query, Server &server);
    ~Response();

    // utils
    void    error(int status);
    std::string statusLine(int status);
    void    getFile();
    void    moveFile();
    void    setAllowedMethodsHeader();
    void    execCGI();
    void    generateAutoindex();
    std::string format(std::string str);
    char    **get_cgi_env();
    // void    digestPath();

    // commands
    void    _get();
    void    _head();
    void    _post();
    void    _put();
    void    _delete();
    void    _connect();
    void    _options();
    void    _trace();
    void    _patch();

    void    execute();
    void    set_location();
    std::string render();

    private:
    RequestParser       &query;
    Server              &server;
    ResponseHeader      header;
    int                 status;
    std::string         content;
    unsigned int        loc;
};

#endif