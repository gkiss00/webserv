#include "ConfigFileReader.hpp"

ConfigFileReader::ConfigFileReader(){}
ConfigFileReader::~ConfigFileReader(){}

std::vector<Server> ConfigFileReader::readFile(std::string path){
    std::vector<Server>     servers;
    std::string content = file_to_string(path);
    std::vector<std::string> lines = split(content, "\n");

    for (unsigned int i = 0; i < lines.size(); ++i){
        std::string line = trim(lines.at(i), " ");

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

Server ConfigFileReader::getServer(std::vector<std::string> lines, unsigned int *i){
    Server server;

    while(*i < lines.size()){
        //std::cout << *i << std::endl;
        std::string line = trim(lines.at(*i), " ");
        //std::cout << line << std::endl;

        if(line.compare("") == 0){

        }else if(line[0] == '#'){
        }else{
            std::vector<std::string> args = split(line, " ");
            //for (unsigned int j = 0; j < args.size(); ++j){
                //std::cout << j << " : " <<args[j] << std::endl;
            //}
            if (args[0].compare("listen") == 0){
                std::cout << args[1] << std::endl;
                server.listen = std::stoi(args[1]);
            }else if (args[0].compare("server_name") == 0){
                for (unsigned int j = 1; j < args.size(); ++j){
                    server.server_names.push_back(trim(args[j], ";"));
                }
            }else if (args[0].compare("client_max_body_size") == 0){
                server.client_max_body_size = std::stoi(args[1]);
            }else if (args[0].compare("root") == 0){
                server.root = trim(args[1], ";");
            }else if (args[0].compare("autoindex") == 0){
                server.autoindex = (trim(args[1], ";").compare("on") == 0);
            }else if (args[0].compare("methods") == 0){
                ++*i;
                args = split(lines.at(*i), " ");
                for (unsigned int j = 0; j < args.size(); ++j){
                    server.methodes.push_back(trim(args[j], ";"));
                }
                ++*i;
            }else if (args[0].compare("metavariables") == 0){
                ++*i;
                args = split(lines.at(*i), " ");
                for (unsigned int j = 0; j < args.size(); ++j){
                    server.metavariables.push_back(trim(args[j], ";"));
                }
                ++*i;
            }else if (args[0].compare("error_page") == 0){
                server.error_pages.insert(std::pair<int, std::string>(std::stoi(args[1]), trim(args[2], ";")));
            }else if (args[0].compare("}") == 0){
                break;
            }else{
                
            }
        }
        ++*i;
    }
    return (server);
}