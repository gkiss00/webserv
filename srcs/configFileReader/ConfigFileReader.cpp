#ifndef CONFIG_FILE_READER_HPP
#define CONFIG_FILE_READER_HPP

#include "webserv.hpp"
#include "Server.hpp"

class ConfigFileReader{
    public:
        ConfigFileReader();
        ~ConfigFileReader();

        std::vector<Server> readFile(std::string path);

    private:
        Server getServer(std::vector<std::string> lines, int *i);

};

ConfigFileReader::ConfigFileReader(){}
ConfigFileReader::~ConfigFileReader(){}

std::vector<Server> ConfigFileReader::readFile(std::string path){
    int                     level = 0;
    std::vector<Server>     servers;
    std::string content = file_to_string(path);
    std::vector<std::string> lines = split(content, "\n");

    for (int i = 0; i < lines.size(); ++i){
        std::string line = trim(lines.at(i));

        if(line.compare("") == 0){

        }else if(line[0] == '#'){
        }else{
            std::vector<std::string> args = split(line, " ");
            if (args[0].compare("server") == 0){
                servers.push_back(getServer(lines, &(++i)));
            }
        }
    }

    return (servers);
}

Server ConfigFileReader::getServer(std::vector<std::string> lines, int *i){
    Server server;

    while(*i < lines.size()){
        std::string line = trim(lines.at(*i));

        if(line.compare("") == 0){

        }else if(line[0] == '#'){
        }else{
            std::vector<std::string> args = split(line, " ");
            if (args[0].compare("listen") == 0){
                server.listen = std::stoi(args[1]);
            }else if (args[0].compare("server_name") == 0){
                for (int j = 1; j < args.size(); ++j){
                    server.server_names.push_back(args[j]);
                }
            }else if (args[0].compare("client_max_body_size") == 0){
                server.client_max_body_size = std::stoi(args[1]);
            }else if (args[0].compare("root") == 0){
                server.root = args[1];
            }else if (args[0].compare("autoindex") == 0){
                server.autoindex = (args[1].compare("on") == 0);
            }else if (args[0].compare("methods") == 0){
                ++*i;
                args = split(lines.at(*i), " ");
                for (int j = 0; j < args.size(); ++j){
                    server.methodes.push_back(args[j]);
                }
                ++*i;
            }else if (args[0].compare("metavariables") == 0){
                ++*i;
                args = split(lines.at(*i), " ");
                for (int j = 0; j < args.size(); ++j){
                    server.metavariables.push_back(args[j]);
                }
                ++*i;
            }else if (args[0].compare("}") == 0){
                ++*i;
                break;
            }else{
                
            }
        }
        ++*i;
    }
    return (server);
}

#endif