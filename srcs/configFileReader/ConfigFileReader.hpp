#ifndef CONFIG_FILE_READER_HPP
#define CONFIG_FILE_READER_HPP

#include "webserv.hpp"
#include "Server.hpp"
class Server;

#define CONFIG_FILE_PTAH "srcs/config/webserv.conf"

class ConfigFileReader{
    public:
        ConfigFileReader();
        ~ConfigFileReader();

        std::vector<Server> readFile(std::string path);

    private:
        Server getServer(std::vector<std::string> lines, unsigned int *i);

};

#endif