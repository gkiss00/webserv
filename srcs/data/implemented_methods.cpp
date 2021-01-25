#include "webserv.hpp"

// GET HEAD POST PUT DELETE CONNECT OPTIONS TRACE PATCH

std::vector<std::string> implementedMethods()
{
    std::vector<std::string> im;

    im.push_back("GET");
    im.push_back("HEAD");
    im.push_back("POST");
    im.push_back("PUT");
    im.push_back("DELETE");
    im.push_back("CONNECT");
    im.push_back("OPTIONS");
    im.push_back("TRACE");
    im.push_back("PATCH");
    return im;
}