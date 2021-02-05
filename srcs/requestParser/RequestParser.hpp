#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

#include "webserv.hpp"

#define URL_MAX_SIZE 2000
#define REQUEST_MAX_SIZE 1000000000
#define CONTENT_LENGHT_MAX 1000000000

class request_exception : public std::exception {

    public:

    request_exception(std::string msg, int error_status) {
        this->_msg = msg;
        this->_error_status = error_status;
    }

    const char   *what () const throw () {
        return this->_msg.c_str();
    }

    int         get_error_status() const {
        return this->_error_status;
    }

    // ~request_exception() _NOEXCEPT {}
    ~request_exception() {}

    private:
    int         _error_status;
    std::string _msg;
};

class RequestParser
{
    public:
    RequestParser(std::string request);
    ~RequestParser();

    std::string full_request;
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