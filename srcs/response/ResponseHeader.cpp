#include "ResponseHeader.hpp"

ResponseHeader::ResponseHeader() {
    content["Allow"] = "GET DELETE HEAD";
    content["Content-Language"] = "fr, en";
    content["Content-Length"] = "";
    content["Content-Location"] = "";
    content["Content-Type"] = "";
    content["Date"] = "";
    content["Last-Modified"] = "";
    content["Location"] = "";
    content["Server"] = "webserv/19";
    content["Transfer-Encoding"] = "";
    updateDate();
}

ResponseHeader::~ResponseHeader() {}

void ResponseHeader::updateDate() {
    addHeader("Date", string_date());
}

void ResponseHeader::addHeader(std::string key, std::string value) {
    content[key] = value;
}

std::string ResponseHeader::toString() {
    std::string str("");
    std::map<std::string, std::string>::iterator it;

    for (it = content.begin(); it != content.end(); it++)
    {
        if ((*it).second == "")
            continue ;
        str += ((*it).first + " : " + (*it).second + "\n");
    }
    return str;
}
