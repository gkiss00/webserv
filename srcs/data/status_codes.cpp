#include "webserv.hpp"

std::map<int,std::string> statusCodes()
{
    std::map<int,std::string> status;

    status[100] = "Continue";
    status[101] = "Switching Protocols";
    status[200] = "OK";
    status[201] = "Created";
    status[202] = "Accepted";
    status[203] = "Non-Authoritative Information";
    status[204] = "No Content";
    status[205] = "Reset Content";
    status[206] = "Partial Content";
    status[300] = "Multiple Choices";
    status[301] = "Moved Permanently";
    status[302] = "Found";
    status[303] = "See Other";
    status[304] = "Not Modified";
    status[305] = "Use Proxy";
    status[307] = "Temporary Redirect";
    status[400] = "Bad Request";
    status[401] = "Unauthorized";
    status[402] = "Payment Required";
    status[403] = "Forbidden";
    status[404] = "Not Found";
    status[405] = "Method Not Allowed";
    status[406] = "Not Acceptable";
    status[407] = "Proxy Authentication Required";
    status[408] = "Request Time-out";
    status[409] = "Conflict";
    status[410] = "Gone";
    status[411] = "Length Required";
    status[412] = "Precondition Failed";
    status[413] = "Request Entity Too Large";
    status[414] = "Request-URI Too Large";
    status[415] = "Unsupported Media Type";
    status[416] = "Requested range not satisfiable";
    status[417] = "Expectation Failed";
    status[500] = "Internal Server Error";
    status[501] = "Not Implemented";
    status[502] = "Bad Gateway";
    status[503] = "Service Unavailable";
    status[504] = "Gateway Time-out";
    status[505] = "HTTP Version not supported";

    return status;
}
