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

void        Response::moveFile()
{
    if (status == 200){
        // move to bin folder when root will be
        if (rename(this->query.path.c_str(), std::string(this->query.path + ".del").c_str()) != 0){
		    perror("File moving failed: ");
        }
    }
}

void        Response::getFile() {
    if (status == 200)
        content = file_to_string(query.path);
    else
        content = file_to_string(error_pages + std::to_string(status) + ".html");
}

void        Response::parse() {
    if (query.command == "GET")
    {
        getStatus();
        getFile();
    }else if (query.command == "DELETE"){
        this->getStatus();
        this->moveFile();
        this->content = "";
    }
}

std::string Response::render() {

    parse();

    return std::string("HTTP/1.1 ") + std::to_string(status) + "CODE_STATUS (tmp)" + "\n"
        + std::string("Date: ") + string_date() + "\n"
        + std::string("Content-Type: ") + content_type + "\n"
        + std::string("Content-Length: ") + std::to_string(content.size()) + "\n\n"
        + content + "\r\n";
}
