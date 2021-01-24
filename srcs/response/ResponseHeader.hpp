#ifndef RESPONSE_HEADERS_HPP
# define RESPONSE_HEADERS_HPP

# include <map>
# include <string>
# include "tools.hpp"

class ResponseHeader
{
    public:
    ResponseHeader();
    ~ResponseHeader();

    void addHeader(std::string key, std::string value);
    std::string toString();

    private:
    void updateDate();
    std::map<std::string, std::string> content;
};

#endif