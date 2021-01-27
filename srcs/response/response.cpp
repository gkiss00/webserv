#include "Response.hpp"

#define DIRECTORY_STATS 040000
#define DEBUG 1

Response::Response(RequestParser &query, Server &server)
: query(query), server(server) {
    setAllowedMethodsHeader();
    chdir(server.root.c_str());
}

Response::~Response() {}

//  _   _  _____  ___  _      ____  
// | | | ||_   _||_ _|| |    / ___| 
// | | | |  | |   | | | |    \___ | 
// | |_| |  | |   | | | |___  ___) |
//  \___/   |_|  |___||_____||____/ 
                                 

void        Response::error(int status) {
    this->status = status;
    query.path = server.error_pages[status];
    getFile();
}

std::string Response::statusLine(int status) {
    std::string sl(string("HTTP/1.1 ") + std::to_string(status) + " " + statusCodes()[status] + "\n");
    return sl;
}

// header update
void        Response::setAllowedMethodsHeader() {
    std::string allowed;
    for (unsigned int i = 0; i < server.methodes.size(); ++i){
        allowed += server.methodes.at(i);
        if (i != server.methodes.size() - 1)
            allowed += ", ";
    }
    header.addHeader("Allowed-Methods", allowed);
}

void        Response::getFile() {
    struct stat stats;

    content = "\r\n" + file_to_string(query.path);
    stat(query.path.c_str(), &stats);
    header.addHeader("Content-Length", std::to_string(content.size()));
    header.addHeader("Content-Type", "text/html");
    header.addHeader("Last-Modified", string_date(gmtime(&stats.st_ctime)));
    header.addHeader("Content-Location", query.path);
    header.addHeader("Content-Type", fileExtension()[query.path.substr(query.path.find_last_of(".") + 1)]);
}

//  __  __  _____  _____  _   _   ___   ____   ____  
// |  \/  || ____||_   _|| | | | / _ \ |  _ \ / ___| 
// | |\/| ||  _|    | |  | |_| || | | || | | |\___ | 
// | |  | || |___   | |  |  _  || |_| || |_| | ___) |
// |_|  |_||_____|  |_|  |_| |_| \___/ |____/ |____/ 
//                                                   

//               _   
//   __ _   ___ | |_ 
//  / _` | / _ \| __|
// | (_| ||  __/| |_ 
//  \__, | \___| \__|
//  |___/            

void        Response::_get() {
    struct stat stats;
    
    if (stat(query.path.c_str(), &stats) == 0)
    {
        if (stats.st_mode & DIRECTORY_STATS)
        {
            std::cout << ">>" << query.path << "\n";
            if (query.path[query.path.size() - 1] != '/')
                query.path += "/";
            if (server.default_file != "") {
                query.path += server.default_file;
                _get(); // maybe readdir for autoindex on?
            }
        }
        status = 200;
        getFile();
    }
    else
    {
        error(404);
    }
}

//  _                       _ 
// | |__    ___   __ _   __| |
// | '_ \  / _ \ / _` | / _` |
// | | | ||  __/| (_| || (_| |
// |_| |_| \___| \__,_| \__,_|
//                            

void        Response::_head() {
    _get();
    content = "";
}

//                _   
//  _ __   _   _ | |_ 
// | '_ \ | | | || __|
// | |_) || |_| || |_ 
// | .__/  \__,_| \__|
// |_|                

void        Response::_put() {
    
}

//                                        _   
//   ___  ___   _ __   _ __    ___   ___ | |_ 
//  / __|/ _ \ | '_ \ | '_ \  / _ \ / __|| __|
// | (__| (_) || | | || | | ||  __/| (__ | |_ 
//  \___|\___/ |_| |_||_| |_| \___| \___| \__|
//                                            

void        Response::_connect() {
    
}

//                _    _                    
//   ___   _ __  | |_ (_)  ___   _ __   ___ 
//  / _ \ | '_ \ | __|| | / _ \ | '_ \ / __|
// | (_) || |_) || |_ | || (_) || | | |\__ \
//  \___/ | .__/  \__||_| \___/ |_| |_||___/
//        |_|                               

void        Response::_options() {
    
}

//  _                          
// | |_  _ __  __ _   ___  ___ 
// | __|| '__|/ _` | / __|/ _ \
// | |_ | |  | (_| || (__|  __/
//  \__||_|   \__,_| \___|\___|
//                             

void        Response::_trace() {
    
}

//                _         _     
//  _ __    __ _ | |_  ___ | |__  
// | '_ \  / _` || __|/ __|| '_ | 
// | |_) || (_| || |_| (__ | | | |
// | .__/  \__,_| \__|\___||_| |_|
// |_|                            

void        Response::_patch() {
    
}

//      _        _        _        
//   __| |  ___ | |  ___ | |_  ___ 
//  / _` | / _ \| | / _ \| __|/ _ \
// | (_| ||  __/| ||  __/| |_|  __/
//  \__,_| \___||_| \___| \__|\___|
//                                 

void        Response::_delete() {
    _get();
    moveFile();
    content = "";
}

void        Response::moveFile()
{
    if (status == 200) {
        // move to bin folder when root will be
        if (rename(this->query.path.c_str(), string(this->query.path + ".del").c_str()) != 0){
            perror("File moving failed: ");
        }
    }
}

//                     _   
//  _ __    ___   ___ | |_ 
// | '_ \  / _ \ / __|| __|
// | |_) || (_) |\__ \| |_ 
// | .__/  \___/ |___/ \__|
// |_|                     

void        Response::_post() {
    status = 200;
    this->execCGI();
}

void        Response::execCGI()
{
    int		fd[2]; // 0 = read 1 = write
    int		fd_write_from_child[2];

    if (status == 200) {
        pipe(fd);
        pipe(fd_write_from_child);

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed: ");
            return ;
        } else if (pid == 0) {
            close(fd[1]);
            close(fd_write_from_child[0]);

            dup2(fd[0], STDIN_FILENO);
            dup2(fd_write_from_child[1], STDOUT_FILENO);

            // char    arg1[] = std::string(servers[0].root + this->query.path).c_str();
            // char    arg1[] = "/Users/corentin/Documents/Programmation/19/webserv/srcs/pages/cgi-bin/add.cgi";
            char    *args[2];
            args[0] = (char *)std::string(servers[0].root + "/" + this->query.path).c_str();
            std::cerr << "\033[32;1m args[0] = " << args[0] << std::endl;
            std::cerr << "\033[0m";
            args[1] = NULL;

            char env1[] = "REQUEST_METHOD=POST";
            char *env[2];
            env[0] = env1;
            env[1] = NULL;

            execve(args[1], args, env);
            perror("execve failed: ");
            return ;
        } else {
            close(fd[0]);
            close(fd_write_from_child[1]);

            // std::cout << "---- send to child ----" << std::endl;
            // std::cout << this->query.body << std::endl;
            write(fd[1], this->query.body.c_str(), this->query.body.size());
            close(fd[1]);

            std::string request;
            char buf[1001];
            int ret;
            wait(NULL);

            while ((ret = read(fd_write_from_child[0], buf, 1000)) > 0){
                buf[ret] = '\0';
                request += buf;
            } 
            close(fd_write_from_child[0]);
            
            // std::cout << "---- get from child ----" << std::endl;
            // std::cout << buff << std::endl;
            // std::cout << "---- end ----" << std::endl;
            this->content = std::string(buf);
        }
    }
}

//  ____   _____  _   _  ____   _____  ____  
// |  _ \ | ____|| \ | ||  _ \ | ____||  _ | 
// | |_) ||  _|  |  \| || | | ||  _|  | |_) |
// |  _ < | |___ | |\  || |_| || |___ |  _ < 
// |_| \_\|_____||_| \_||____/ |_____||_| |_|
//                                           

//
// Below are the main functions. These functions can call particular commands
// depending on the query.command. They can also call error(status_code) at any
// time.
// 3 fields are to be filled: status, header, content.
//

void        Response::execute() {
    if (query.command == "GET") {
        this->_get();
    } else if (query.command == "HEAD") {
        this->_head();
    } else if (query.command == "POST") {
        this->_post();
    } else if (query.command == "PUT") {
        this->_put();
    } else if (query.command == "DELETE") {
        this->_delete();
    } else if (query.command == "CONNECT") {
        this->_connect();
    } else if (query.command == "OPTIONS") {
        this->_options();
    } else if (query.command == "TRACE") {
        this->_trace();
    } else if (query.command == "PATCH") {
        this->_patch();
    } 
}

string Response::render() {
    std::vector<std::string> allImplementedMethods = implementedMethods();

    if (!std::count(allImplementedMethods.begin(), allImplementedMethods.end(), query.command))
        error(501); // not implemented

    else if (!std::count(server.methodes.begin(), server.methodes.end(), query.command))
        error(405); // not authorized

    else
        execute();

    std::string response(statusLine(status) + header.toString() + content + "\r\n");

#ifdef DEBUG
    std::cout << "_____RESPONSE_____" << std::endl;
    std::cout << response;
    std::cout << "_____        _____" << std::endl;
#endif

    return response;
}
