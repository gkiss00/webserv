#include "Client.hpp"

Client::

Client::Client(int server_sd) : server_sd(server_sd) {}

Client::~Client() {}

void Client::add_content(std::string new_content) {
    content += new_content;
}

void Client::rm_content() {
    content = "";
}

bool Client::is_ready() {
    if ((content.find("chunked") == std::string::npos // we find that it is not chunked 
    && content.find("\r\n\r\n") != std::string::npos)
    || content.find("0\r\n\r\n") != std::string::npos) {
        return true;
    }
    return false;
}

void Client::clear() {
    content.clear();
}

std::string Client::get_content() {
    return content;
}

