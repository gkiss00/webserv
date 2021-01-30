#include "NewConfigFileReader.hpp"

NewConfigFileReader::NewConfigFileReader(){}

NewConfigFileReader::~NewConfigFileReader(){}

std::vector<Server> NewConfigFileReader::read(std::string path){
    std::vector<Server> servers;
    std::string content = file_to_string(path);
    std::vector<std::string> lines = split(content, "\n"); //get all lines of the file

    for (unsigned int i = 0; i < lines.size(); ++i){
        std::string line = trim(lines.at(i), " "); //get the current line

        if(line.compare("") == 0){ //if line is empty : PASS
        }else if(line[0] == '#'){ // if line is a comment : PASS
        }else{
            std::vector<std::string> args = split(line, " "); // get all args of the line
            if (args[0].compare("server") == 0){ //get the server conf
                servers.push_back(getServer(lines, &(++i)));
            }
        }
    }
    return servers;
}

Server  NewConfigFileReader::getServer(std::vector<std::string> lines, unsigned int *i){
    Server server;
    while (*i < lines.size()){
        std::string line = trim(lines.at(*i), " "); //get the current line

        if(line.compare("") == 0){ //if line is empty : PASS
        }else if(line[0] == '#'){ // if line is a comment : PASS
        }else if(line[0] == '}'){ // if end  : BREAK
            break;
        }else{
            std::vector<std::string> args = split(line, " "); // get all args of the line

            if (args[0].compare("listen") == 0){ // get the PORT of the server
                server.listen = std::stoi(args[1]);
            }else if (args[0].compare("server_name") == 0){ // get the SERVER_NAMES of the server
                for (unsigned int j = 1; j < args.size(); ++j){
                    server.server_names.push_back(trim(args[j], ";"));
                }
            }else if (args[0].compare("client_max_body_size") == 0){ // get the CLIENT_MAX_BODYSIZE of the server
                server.client_max_body_size = std::stoi(args[1]);
            }else if (args[0].compare("error_page") == 0){ // get the ERROR_PAGES of the server
                server.error_pages.insert(std::pair<int, std::string>(std::stoi(args[1]), trim(args[2], ";")));
            }else if (args[0].compare("metavariables") == 0){ // get METAVAR of the server
                server.metavariables = getMetavar(lines, i);
            }else if (args[0].compare("location") == 0){ // get LOCATION of the server
                server.locations.push_back(getLocation(lines, i));
            }
        }
        ++*i;
    }
    return (server);
}

std::vector<std::string> NewConfigFileReader::getMetavar(std::vector<std::string> lines, unsigned int *i){
    std::vector<std::string> metavar;
    
    ++*i;
    while(*i < lines.size()){
        std::string line = trim(lines.at(*i), " "); // get the current line
        
        if(line.compare("") == 0){ //if line is empty : PASS
        }else if(line[0] == '#'){ // if line is a comment : PASS
        }else if(line[0] == '}'){ // if close : BREAK
            break;
        }else{
            std::vector<std::string> args = split(lines.at(*i), " ");
            for (unsigned int j = 0; j < args.size(); ++j){
                metavar.push_back(trim(args[j], ";"));
            }
        }
        ++*i;
    }
    return (metavar);
}

Location    NewConfigFileReader::getLocation(std::vector<std::string> lines, unsigned int *i){
    Location location;

    location.dir = split(lines.at(*i), " ")[1]; // get the DIR
    ++*i;
    while(*i < lines.size()){
        std::string line = trim(lines.at(*i), " "); // get the current line

        if(line.compare("") == 0){ //if line is empty : PASS
        }else if(line[0] == '#'){ // if line is a comment : PASS
        }else if(line[0] == '}'){ // if close : BREAK
            break;
        }else{
            std::vector<std::string> args = split(line, " "); // get all args of the line

            if(args[0].compare("root") == 0){ // get the ROOT
                location.root = trim(args[1], ";");
            }else if (args[0].compare("autoindex") == 0){ // get AUTOINDEX
                location.autoindex = (trim(args[1], ";").compare("on") == 0);
            }else if (args[0].compare("index") == 0){ // get DEFAULT_FILE
                location.default_file = trim(args[1], ";");
                std::cout << "0000000000000000" << std::endl;
                std::cout << location.default_file << std::endl;
            }else if (args[0].compare("cgi") == 0){ // get CGI
                location.cgi.insert(getCGI(lines, i));
            }else if (args[0].compare("upload") == 0){ // get UPLOAD location
                location.upload = getUpload(lines, i);
            }else if (args[0].compare("methods") == 0){ // get UPLOAD location
                location.methods = getMethods(lines, i);
            }
        }
        ++*i;
    }
    return (location);
}

std::vector<std::string> NewConfigFileReader::getMethods(std::vector<std::string> lines, unsigned int *i){
    std::vector<std::string> methods;

    ++*i;
    while(*i < lines.size()){
        std::string line = trim(lines.at(*i), " "); // get the current line


        if(line.compare("") == 0){ //if line is empty : PASS
        }else if(line[0] == '#'){ // if line is a comment : PASS
        }else if(line[0] == '}'){ // if close : BREAK
            break;
        }else{
            std::vector<std::string> args = split(line, " ");
            for (unsigned int j = 0; j < args.size(); ++j){
                methods.push_back(trim(args.at(j), ";"));
            }
        }
        ++*i;
    }
    return (methods);
}

std::pair<std::string, std::string> NewConfigFileReader::getCGI(std::vector<std::string> lines, unsigned int *i){
    std::string type;
    std::string path;

    type = trim(split(lines.at(*i), " ")[1], "{"); // get the cgi type
    ++*i;
    while(*i < lines.size()){
        std::string line = trim(lines.at(*i), " "); // get the current line


        if(line.compare("") == 0){ //if line is empty : PASS
        }else if(line[0] == '#'){ // if line is a comment : PASS
        }else if(line[0] == '}'){ // if close : BREAK
            break;
        }else{
            path = trim(split(line, " ")[1]); // get the cgi path
            if (path.back() == ';'){
                path.pop_back();
            }
        }
        ++*i;
    }
    return(std::pair<std::string, std::string>(type, path));
}

std::string NewConfigFileReader::getUpload(std::vector<std::string> lines, unsigned int *i){
    std::string upload;

    ++*i;
    while(*i < lines.size()){
        std::string line = trim(lines.at(*i), " "); // get the current line


        if(line.compare("") == 0){ //if line is empty : PASS
        }else if(line[0] == '#'){ // if line is a comment : PASS
        }else if(line[0] == '}'){ // if close : BREAK
            break;
        }else{
            upload = trim(split(line, " ")[1], ";"); // get the cgi path
        }
        ++*i;
    }
    return (upload);
}