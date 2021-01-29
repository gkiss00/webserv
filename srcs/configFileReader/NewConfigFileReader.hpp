#ifndef NEWCONFIGFILEREADER_HPP
#define NEWCONFIGFILEREADER_HPP

# include "webserv.hpp"

class Server;
class Location;

class NewConfigFileReader {
    public:
        NewConfigFileReader();
        ~NewConfigFileReader();

        std::vector<Server>     read(std::string path);
    private:
        Server  getServer(std::vector<std::string> lines, unsigned int *i);
        Location    getLocation(std::vector<std::string> lines, unsigned int *i);
        std::vector<std::string> getMethods(std::vector<std::string> lines, unsigned int *i);
        std::vector<std::string> getMetavar(std::vector<std::string> lines, unsigned int *i);
        std::pair<std::string, std::string> getCGI(std::vector<std::string> lines, unsigned int *i);
        std::string getUpload(std::vector<std::string> lines, unsigned int *i);
};

#endif