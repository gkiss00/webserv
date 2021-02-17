#include "PostHeader.hpp"

// https://en.wikipedia.org/wiki/Common_Gateway_Interface

PostHeader::PostHeader(Server &server, Location &loc, RequestParser &request) {
    // Default values
    content["AUTH_TYPE"] = "Accept";
    content["CONTENT_LENGTH"] = std::to_string(request.body.size());
    content["CONTENT_TYPE"] = "text/plain";
    content["GATEWAY_INTERFACE"] = "CGI/1.1";
    content["PATH_INFO"] = loc.dir + "/" +request.path;
    content["PATH_TRANSLATED"] = loc.root + request.path;
    content["QUERY_STRING"] = "''";
    content["REMOTE_ADDR"] = "''";
    content["REMOTE_IDENT"] = "''";
    content["REMOTE_USER"] = "''";
    content["REQUEST_METHOD"] = "POST";
    content["REQUEST_URI"] = loc.dir + "/" + request.path;
    std::string script_path = loc.cgi[request.path.substr(request.path.rfind("."))]; 
    content["SCRIPT_NAME"] = script_path.substr(script_path.rfind("/") + 1);
    content["SERVER_NAME"] = "''";
    content["SERVER_PORT"] = std::to_string(server.listen);
    content["SERVER_PROTOCOL"] = "HTTP/1.1";
    content["SERVER_SOFTWARE"] = "webserv s19";
 
    // HTTP_HEADERS
    std::map<std::string, std::string>::iterator it = request.headers.begin();
    std::map<std::string, std::string>::iterator ite = request.headers.end();
    while (it != ite){
        std::string header_name = "HTTP_" + it->first;
        std::transform(header_name.begin(), header_name.end(), header_name.begin(), ::toupper);
        std::replace(header_name.begin(), header_name.end(), '-', '_');

        content[header_name] = it->second;
        ++it;
    }

    // Defined Values
    if (request.headers.count("Auth-Type"))
        content["AUTH_TYPE"] = request.headers["Auth-Type"];

    if (request.headers.count("?")) {
        content["PATH_INFO"] = content["PATH_INFO"].substr(0, request.path.find("?"));
        content["PATH_TRANSLATED"] = content["PATH_INFO"].substr(0, request.path.find("?"));
        content["QUERY_STRING"] = request.path.substr(request.path.find("?") + 1);
    }

    if (request.headers.count("Host")) {
        content["REMOTE_ADDR"] = request.headers["Host"].substr(0, request.headers["Host"].find(":"));
        content["SERVER_NAME"] = request.headers["Host"].substr(0, request.headers["Host"].find(":"));
    }

    if (request.headers.count("Remote-User"))
        content["REMOTE_USER"] = request.headers["Remote-User"];

    if (request.headers.count("Remote-Ident"))
        content["REMOTE_IDENT"] = request.headers["Remote-Ident"];
}

PostHeader::~PostHeader() {}

void PostHeader::addHeader(std::string key, std::string value) {
    content[key] = value;
}

char **PostHeader::toTable() {
    std::string tmp;
    std::map<std::string, std::string>::iterator it;
    char **table = (char **)malloc((content.size() + 1) * sizeof(char*));
    size_t i = 0;

    for (it = content.begin(); it != content.end(); it++)
    {
        tmp = ((*it).first + "=" + (*it).second);
        char *cstr = (char *)malloc((tmp.size() + 1) * sizeof(char));
        for (size_t j = 0; j <= tmp.size(); ++j) {
            cstr[j] = tmp[j];
        }

        table[i++] = cstr;
    }
    table[i] = NULL;

    return table;
}

void PostHeader::print() {
    for (std::map<std::string, std::string>::iterator it = content.begin(); it != content.end(); ++it) {
        std::cout << it->first << "=" << it->second << std::endl;
    }
}
