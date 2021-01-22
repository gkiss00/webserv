#include "Response.hpp"

Response::Response(RequestParser &query)
: query(query), error_pages("srcs/pages/error_page/") {};

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
        query.path = error_pages + std::to_string(status) + ".html";
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
    struct stat stats;

    content = file_to_string(query.path);
    stat(query.path.c_str(), &stats);
    header.addHeader("Content-Length", std::to_string(content.size()));
    header.addHeader("Content-Type", "text/html");
    header.addHeader("Last-Modified", string_date(gmtime(&stats.st_ctime)));
    header.addHeader("Content-Location", query.path);
}

void        Response::parse() {
    if (query.command == "GET" || query.command == "HEAD") {
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
        + header.toString() + "\n"
        + ((query.command == "GET") ? content : "") + "\r\n";
}
