#ifndef POST_HEADERS_HPP
# define POST_HEADERS_HPP

# include "webserv.hpp"

class Server;
class Location;

class PostHeader
{
    public:
    PostHeader(Server &server, Location &loc, RequestParser &request);
    ~PostHeader();

    void addHeader(std::string key, std::string value);
    char **toTable();
    void print();

    private:
    std::map<std::string, std::string> content;
};

#endif