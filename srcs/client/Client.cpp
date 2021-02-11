#include "Client.hpp"

Client::

Client::Client(int server_sd) : server_sd(server_sd) {}

Client::~Client() {}

void Client::add_content(std::string new_content) {
    content += new_content;
}

bool Client::is_ready() {
    if ((content.find("chunked") == std::string::npos // we find that it is not chunked 
    && content.find("\r\n\r\n") != std::string::npos)
    || content.find("0\r\n\r\n") != std::string::npos) {
        return true;
    }
    return false;
}

void Client::clear_content() {
    content.clear();
}

std::string Client::get_content() {
    return content;
}

void Client::add_response(std::string new_response, bool new_is_put) {
    response = new_response;
    is_put = new_is_put;
}

void Client::clear_response() {
    response.clear();
}

std::string Client::get_response() {
    return response;
}

bool Client::get_is_put() {
    return is_put;
}

