#include "Response.hpp"

Response::Response(RequestParser &query)
: query(query),
error_pages("srcs/pages/error_page/"),
status(0),
content(string()),
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
    if (status == 200) {
        // move to bin folder when root will be
        if (rename(this->query.path.c_str(), string(this->query.path + ".del").c_str()) != 0){
            perror("File moving failed: ");
        }
    }
}

void        Response::getFile() {
    (status / 100 == 2)
    ? content = file_to_string(query.path)
    : content = file_to_string(error_pages + std::to_string(status) + ".html");
}

void        Response::parse() {
    if (query.command == "GET") {
        getStatus();
        getFile();
    } else if (query.command == "DELETE") {
        this->getStatus();
        this->moveFile();
        this->content = "";
    }
}

string Response::render() {

    parse();

    return string("HTTP/1.1 ") + std::to_string(status) + " " + statusCodes()[status] + "\n"
        + string("Date: ") + string_date() + "\n"
        + string("Content-Type: ") + content_type + "\n"
        + string("Content-Length: ") + std::to_string(content.size()) + "\n\n"
        + content + "\r\n";
}
