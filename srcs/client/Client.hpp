// We can use them as a map of fds.

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class Client {
    
    public:
        Client(int server_sd = 1000);
        ~Client();
        void add_content(std::string new_content);
        void clear_content();
        std::string get_content();
        bool is_ready();

        void add_response(std::string new_response, bool new_is_put);
        void clear_response();
        std::string get_response();

        bool get_is_put();

// variables

        int         server_sd;

    private:
        std::string content;
        std::string response;
        bool        is_put;
};

#endif