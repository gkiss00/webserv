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

    content = "\n" + file_to_string(query.path);
    stat(query.path.c_str(), &stats);
    header.addHeader("Content-Length", std::to_string(content.size()));
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

void        Response::generateAutoindex()
{
    DIR *dir;
    struct dirent *ent;

    content += "\n<h1>Index of " + query.path + "</h1>\n";
    if ((dir = opendir(query.path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            struct stat stats;
            std::string filename(ent->d_name);

            content += "<p><a href=\"/" + query.path + std::string(filename) + "\">" + filename + "</a>\n<date style=\"color:red\">";
            stat(filename.c_str(), &stats);
            content += string_date(gmtime(&stats.st_ctime)) + "</date>\n";
            content += fileExtension()[filename.substr(filename.find_last_of(".") + 1)];
            content += "<p>\n";
        }
        closedir (dir);
    }
    header.addHeader("Content-Length", std::to_string(content.size()));
}

void        Response::_get() {
    struct stat stats;

    if (query.path == "")
        query.path = ".";
    if (stat(query.path.c_str(), &stats) == 0)
    {
        if (stats.st_mode & DIRECTORY_STATS)
        {
            if (query.path[query.path.size() - 1] != '/')
                query.path += "/";
            if (server.autoindex)
            {
                generateAutoindex();
                status = 200;
                return ;
            }
            if (server.default_file != "") {
                query.path += server.default_file;
                _get();
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

            setenv("REQUEST_METHOD", "POST", 1);
            setenv("QUERY_STRING", this->query.body.c_str(), 1);
            execlp("./srcs/pages/cgi-bin/add.cgi", "./srcs/pages/cgi-bin/add.cgi", NULL); // we must use execve
            perror("execve failed: ");
            return ;
        } else {
            close(fd[0]);
            close(fd_write_from_child[1]);

            usleep(1000);
            // std::cout << "---- send to child ----" << std::endl;
            // std::cout << this->query.body << std::endl;
            write(fd[1], this->query.body.c_str(), this->query.body.size());
            close(fd[1]);

            char buff[10001];
            int ret;
            wait(NULL);
            if ((ret = read(fd_write_from_child[0], buff, 10000)) == -1)
            {
                perror("read fails: ");
            }
            buff[ret] = '\0';
            close(fd_write_from_child[0]);
            
            // std::cout << "---- get from child ----" << std::endl;
            // std::cout << buff << std::endl;
            // std::cout << "---- end ----" << std::endl;
            this->content = std::string(buff);
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
