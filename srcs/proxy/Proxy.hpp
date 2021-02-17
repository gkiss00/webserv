#ifndef PROXY_HPP
# define PROXY_HPP

# include "webserv.hpp"

class Proxy {
    private:
        std::string addr;
        int         port;

    public:
        Proxy();
        ~Proxy();

        std::string getResponse(std::string request);
};

#endif