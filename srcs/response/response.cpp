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

std::string response_delete200(RequestParser &query)
{
    std::cout << "moving " + query.path + "..." << std::endl;
    if (rename(query.path.c_str(), std::string(query.path + ".del").c_str()) != 0){
		perror("File deletion failed: ");
    }else{
		std::cout << "File deleted successfully" << std::endl;
    }
    std::string ret = std::string("HTTP/1.1 200 OK\r\n")
        + "Date: " + string_date() + "\r\n"
        + "Content-Type: text/html" + "\r\n"
        + "Content-Length: " + std::to_string(0) + "\r\n\r\n"
        + "\r\n";

    return ret;
}

std::string response_delete(RequestParser &query)
{
    struct stat stats;

    if (stat(query.path.c_str(), &stats) == 0)
    {
        return response_delete200(query);
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
    if (query.command == "DELETE")
        return response_delete(query);
    return "";
}
