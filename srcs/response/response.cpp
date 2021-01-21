#include "webserv.hpp"

#define ERROR_PAGES_PATH "srcs/pages/error_page/"

std::string response200(RequestParser &query)
{
    std::string file_content = file_to_string(query.path);

    std::string ret = std::string("HTTP/1.1 200 OK\n")
        + "Date: " + string_date() + "\n"
        + "Content-Type: text/html" + "\n"
        + "Content-Length: " + std::to_string(file_content.size()) + "\n\n"
        + file_content
        + "\r\n";

    return ret;
}

std::string response404()
{
    std::string file_content = file_to_string(std::string(ERROR_PAGES_PATH"404.html"));

    std::string ret = std::string("HTTP/1.1 404 PAGE NOT FOUND\n")
        + "Date: " + string_date() + "\n"
        + "Content-Type: text/html" + "\n"
        + "Content-Length: " + std::to_string(file_content.size()) + "\n\n"
        + file_content
        + "\r\n";

    return ret;
}

std::string response_get(RequestParser &query)
{
    struct stat stats;

    if (stat(query.path.c_str(), &stats) == 0)
    {
        return response200(query);
    }
    else
    {
        return response404();
    }
}

std::string response(RequestParser &query)
{
    if (query.command == "GET")
        return response_get(query);
    return "";
}
