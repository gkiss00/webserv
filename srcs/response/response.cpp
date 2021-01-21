#include "Response.hpp"

Response::Response(RequestParser &query)
: query(query),
error_pages("srcs/pages/error_page/"),
status(0),
content(std::string()),
content_type("text/html")
{}

Response::~Response() {}

void        Response::getStatus() {
    struct stat stats;

    if (stat(query.path.c_str(), &stats) == 0)
    {
        status = 200;
    }
    else
    {
        status = 404;
    }
}

void        Response::parse() {
    if (query.command == "GET")
    {
        getStatus();
        getFile();
    }
}

void        Response::getFile() {
    if (status == 200)
        content = file_to_string(query.path);
    else
        content = file_to_string(error_pages + std::to_string(status) + ".html");
}

std::string Response::render() {

    parse();

    return std::string("HTTP/1.1 ") + std::to_string(status) + "CODE_STATUS (tmp)" + "\n"
        + std::string("Date: ") + string_date() + "\n"
        + std::string("Content-Type: ") + content_type + "\n"
        + std::string("Content-Length: ") + std::to_string(content.size()) + "\n\n"
        + content + "\r\n";
}
