// We can use them as a map of fds.

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class Client {
    
    public:
        Client(int server_sd = 1000);
        ~Client();
        void add_content(std::string content);
        bool is_ready();
        void clear();
        std::string get_content();

// variables

        int         server_sd;

    private:
        std::string content;

};

#endif