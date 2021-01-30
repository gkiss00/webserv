#include "Response.hpp"

#define DEBUG 1

Response::Response(RequestParser &query, Server &server)
: query(query), server(server) {
    set_location();
    setAllowedMethodsHeader();
    chdir(server.locations[loc].root.c_str());
}

Response::~Response() {}

//  _   _  _____  ___  _      ____  
// | | | ||_   _||_ _|| |    / ___| 
// | | | |  | |   | | | |    \___ | 
// | |_| |  | |   | | | |___  ___) |
//  \___/   |_|  |___||_____||____/ 
//                                  

void        Response::error(int status) {
    this->status = status;
    query.path = server.error_pages[status];
    getFile();
    // getFile(); // boucle infinie en 404 error vient de get
}

std::string Response::statusLine(int status) {
    std::string sl(string("HTTP/1.1 ") + std::to_string(status) + " " + statusCodes()[status] + "\n");
    return sl;
}

// header update
void        Response::setAllowedMethodsHeader() {
    std::string allowed;
    for (unsigned int i = 0; i < server.locations[loc].methods.size(); ++i){
        allowed += server.locations[loc].methods.at(i);
        if (i != server.locations[loc].methods.size() - 1)
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

    content += "\n<html>\n<head><title>Index of " + trim(query.path, ".") + "</title></head>\n<body>\n";
    content += "<h1>Index of " + trim(query.path, ".") + "</h1><hr><pre>";
    if ((dir = opendir(query.path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            struct stat stats;
            std::string filename(ent->d_name);

            if (filename == ".")
                continue ;
            content += "<a href=\"" + filename + ((is_dir(query.path + filename)) ? "/" : "") + "\">" + filename + ((is_dir(query.path + filename)) ? "/" : "") + "</a>";
            if (filename == "..")
            {
                content += "\n";
                continue ;
            }
            for (size_t i = 0; i < 50 - filename.size() - is_dir(query.path + filename); i++)
                content += ' ';
            stat((query.path + filename).c_str(), &stats);
            content += index_date(gmtime(&stats.st_ctime));
            for (size_t i = 0; i < 20 - ((is_dir(query.path + filename)) ? 1 : width(stats.st_size)); i++)
                content += ' ';
            content += (is_dir(filename)) ? "-" : std::to_string(stats.st_size);
            content += "\n";
        }
        closedir (dir);
    }
    content += "</pre><hr></body>\n</html>\n";
    header.addHeader("Content-Length", std::to_string(content.size()));
}

void        Response::_get() {
    struct stat stats;

    std::cout << "GETTTTTTTTTTTTTTTTT: " << query.path << std::endl;
    if (query.path == "")
        query.path = ".";
    if (stat(query.path.c_str(), &stats) == 0)
    {
        if (is_dir(query.path))
        {
            if (query.path[query.path.size() - 1] != '/')
                query.path += "/";
            if (server.locations[loc].autoindex)
            {
                generateAutoindex();
                status = 200;
                return ;
            }
            if (server.locations[loc].default_file != "") {
                query.path += server.locations[loc].default_file;
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
    status = 200;
    content = query.full_request;
    header.addHeader("Content-Length", std::to_string(content.size()));
    header.addHeader("Content-Type", "message/http");
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
        // move to bin folder when locations[loc].root will be
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

    if (server.locations[loc].cgi.count(split(query.path, ".").back()) != 0){
        this->execCGI();
    }else{
        std::cerr << "---------- post do nothing  -----------" << std::endl;
    }
}

void        Response::execCGI()
{
    pid_t   pid;
    int		fd_write_from_parent[2]; // 0 = read 1 = write
    int		fd_write_from_child[2];
    std::string cgi_path;

    std::cerr << "---------- CGI -----------" << std::endl;
    cgi_path = server.locations[loc].cgi[split(query.path, ".").back()];

    // std::map<std::string, std::string>::iterator it = server.locations[loc].cgi.begin();
    // std::map<std::string, std::string>::iterator ite = server.locations[loc].cgi.end();
    // while (it != ite){
    //     std::cerr << it->first + " and " + it->second << std::endl;
    //     ++it;
    // }
    std::cerr << "---------- CGI path -----------" << std::endl;
    std::cerr << cgi_path << std::endl;
    std::cerr << "------------  ------------" << std::endl;

    status = 404;

    struct stat stats;
    if (stat(cgi_path.c_str(), &stats) == 0)
    {
        if (S_ISREG(stats.st_mode)) {
            status = 200;
        }
    }

    if (status/100 == 2) {
        pipe(fd_write_from_parent);
        pipe(fd_write_from_child);
    
        pid = fork();
        if (pid == -1) {
            perror("fork failed: ");
            return ;
        } else if (pid == 0) {
            close(fd_write_from_parent[1]);
            close(fd_write_from_child[0]);
            dup2(fd_write_from_parent[0], STDIN_FILENO);
            dup2(fd_write_from_child[1], STDOUT_FILENO);

            char    *args[2];
            // args[0] = (char *)std::string("./" + this->query.path).c_str();
            args[0] = (char *)cgi_path.c_str();
            std::cerr << "path = " << args[0] << std::endl;
            args[1] = NULL;

            char *env[5];
            env[0] = (char *)"REQUEST_METHOD=POST";
            env[1] = (char *)"SERVER_PROTOCOL=HTTP/1.1";
            env[2] = (char *)std::string("PATH_INFO=''").c_str();
            env[3] = (char *)std::string("CONTENT_LENGHT=0").c_str();
            env[4] = NULL;

            execve(args[0], args, env);
            perror("execve failed: ");
            close(fd_write_from_parent[0]);
            close(fd_write_from_child[1]);
            exit(-1);
        } else {
            close(fd_write_from_parent[0]);
            close(fd_write_from_child[1]);
            // std::cout << "---- send to child ----" << std::endl;
            // std::cout << this->query.body << std::endl;
            write(fd_write_from_parent[1], this->query.body.c_str(), this->query.body.size());
            close(fd_write_from_parent[1]);

            std::string request;
            char buf[1001];
            int ret;
            int child_status;
            wait(&child_status);
            if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == -1){
                this->content = "";
                return ;
            } 
            while ((ret = read(fd_write_from_child[0], buf, 1000)) > 0){
                buf[ret] = '\0';
                request += buf;
            }
            close(fd_write_from_child[0]);
            
            std::cout << "---- get from child ----" << std::endl;
            std::cout << request << std::endl;
            std::cout << "---- end ----" << std::endl;
            this->content = std::string(request);
            // this->content = "Content-Type: text/html; charset=utf-8\n\n0";
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

void    Response::set_location()
{
    unsigned int compatibility = 0;
    loc = 0;

    for (unsigned int i = 0; i < server.locations.size(); ++i)
    {
        std::string dir = server.locations[i].dir;
        if (dir[0] == '/')
            dir = dir.substr(1);
        std::cout << "dir = " << dir << std::endl;
        if (!query.path.compare(0, dir.size(), dir) && dir.size() >= compatibility)
        {
            loc = i;
            compatibility = dir.size();
        }
    }

    std::cout << "QP = " << query.path << std::endl;
    std::cout << "dir = " << server.locations[loc].dir << std::endl;
    query.path = query.path.substr(server.locations[loc].dir.size() - 1);
    if (query.path[0] == '/') query.path = query.path.substr(1);
    std::cout << "QP = " << query.path << std::endl;
    std::cout << "LOC = " << loc << std::endl;
}

string  Response::render() {
    std::vector<std::string> allImplementedMethods = implementedMethods();

    if (!std::count(allImplementedMethods.begin(), allImplementedMethods.end(), query.command))
        error(501); // not implemented

    else if (!std::count(server.locations[loc].methods.begin(), server.locations[loc].methods.end(), query.command))
        error(405); // not authorized

    else
        execute();

    std::string response(statusLine(status) + header.toString() + content + (content == "" ? "" : ""));

#ifdef DEBUG
    std::cout << "_____RESPONSE_____" << std::endl;
    std::cout << response;
    std::cout << "_____        _____" << std::endl;
#endif

    return response;
}
