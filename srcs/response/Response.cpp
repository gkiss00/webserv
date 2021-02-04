#include "Response.hpp"

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

void        Response::error(int code) {
    this->status = code;
    query.path = server.error_pages[status];
    this->content = "error" + std::to_string(code) + "\n";
    getFile();
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

void        Response::getFile() {
    struct stat stats;

    content = "\n" + file_to_string(query.path) + "\n";
    stat(query.path.c_str(), &stats);
    header.addHeader("Content-Length", std::to_string(content.size() - 1));
    header.addHeader("Last-Modified", string_date(gmtime(&stats.st_ctime)));
    header.addHeader("Content-Location", query.path);
    header.addHeader("Content-Type", fileExtension()[query.path.substr(query.path.find_last_of(".") + 1)]);
}

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
        }
        closedir (dir);
    }
    content += "</pre><hr></body>\n</html>\n\n";
    header.addHeader("Content-Length", std::to_string(content.size() - 1)); // -1 for the preceding \n
}

void        Response::_get() {
    struct stat stats;

    if (query.path == "")
        query.path = ".";
    if (stat(query.path.c_str(), &stats) == 0)
    {
        if (is_dir(query.path))
        {
            if (query.path[query.path.size() - 1] != '/')
                query.path += "/";
            if (query.path == "./")
                query.path = "";
            if (server.locations[loc].autoindex)
            {
                generateAutoindex();
                status = 200;
                return ;
            }
            if (server.locations[loc].default_file != "") {
                query.path += server.locations[loc].default_file;
                _get();
                return ;
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
    struct stat stats;

    mkdir_p(server.locations[loc].upload);
#ifdef DEBUG
    std::cerr << server.locations[loc].upload + query.path << std::endl;
#endif
    if (stat(query.path.c_str(), &stats) == 0) {
        if (query.body != "") {
            create_file(server.locations[loc].upload + query.path, query.body);
            status = 200; // Success
        }
        else {
            create_file(server.locations[loc].upload + query.path, "");
            status = 204; // No Content
        }
    }
    else {
        create_file(server.locations[loc].upload + query.path, query.body);
        status = 201; // Created
    }
    content = "\n";
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

#ifdef DEBUG
    std::cerr << query.path << std::endl;
    std::cerr << loc << std::endl;
    std::cerr << split(query.path, ".").size() << std::endl;
#endif
    if (split(query.path, ".").size() != 0 && server.locations[loc].cgi.count("." + split(query.path, ".").back()) != 0){
        std::cerr << "." + split(query.path, ".").back() << std::endl;
        this->execCGI();
    } else if (query.body.size() == 0) {
        error(204);
    } else if ((int)query.body.size() > server.client_max_body_size) {
        error(413);
    } else {
        error(200);
    }
}

std::string Response::format(std::string str)
{
    std::string ret = "HTTP_";

    std::replace(str.begin(), str.end(), '-', '_');
    for (int i = 0; i < (int)str.size(); ++i){
        ret += toupper(str[i]);
    }
    return (ret);
}

char        **Response::get_cgi_env(){
    char **env = (char **)malloc(100 * sizeof(char *));
    int i = 0;

#ifdef DEBUG
    std::cerr << "---- get_cgi_env ----" << std::endl;
#endif
    env[i++] = strdup("REQUEST_METHOD=POST");
    env[i++] = strdup("SERVER_PROTOCOL=HTTP/1.1");
    // Because you wont call the cgi directly use the full path as PATH_INFO
    env[i++] = strdup("PATH_INFO=/tmp/www/directory/youpi.bla");
#ifdef DEBUG
    std::cerr << "+++++++++++" << server.locations[loc].root + this->query.path << std::endl;
#endif
    env[i++] = strdup(std::string("CONTENT_LENGTH=" + std::to_string(this->query.body.size())).c_str());

    env[i++] = strdup("AUTH_TYPE=''");
    env[i++] = strdup("CONTENT_TYPE=test/file");
    env[i++] = strdup("GATEWAY_INTERFACE=CGI/révision");
    env[i++] = strdup((std::string("PATH_TRANSLATED=") + server.locations[loc].root + this->query.path).c_str());
    env[i++] = strdup("QUERY_STRING=''");
    env[i++] = strdup("REMOTE_ADDR=''");
    env[i++] = strdup("REMOTE_IDENT=''");
    env[i++] = strdup("REMOTE_USER=''");
    env[i++] = strdup("REQUEST_URI=/tmp/www/directory/youpi.bla");
    env[i++] = strdup("SERVER_NAME=''");
    env[i++] = strdup("SCRIPT_NAME=/tmp/cgi-bin/cgi_tester");
    env[i++] = strdup("SERVER_PORT=5000");
    env[i++] = strdup("SERVER_SOFTWARE=HTTP/1.1");

    std::map<std::string, std::string>::iterator it = this->query.headers.begin();
    std::map<std::string, std::string>::iterator ite = this->query.headers.end();
    while (it != ite){
        env[i++] = strdup(std::string(format(it->first) + "=" + it->second).c_str());
        ++it;
    }
    env[i++] = NULL;
    return (env);
}

void        Response::execCGI()
{
    pid_t   pid;
    int     pip[2];
    pip[0] = 0;
    pip[1] = 1;

#ifdef DEBUG
    std::cerr << "---------- CGI -----------" << std::endl;
#endif

    std::string cgi_path = server.locations[loc].cgi["." + split(query.path, ".").back()];

    // std::map<std::string, std::string>::iterator it = server.locations[loc].cgi.begin();
    // std::map<std::string, std::string>::iterator ite = server.locations[loc].cgi.end();
    // while (it != ite){
    //     std::cerr << it->first + " and " + it->second << std::endl;
    //     ++it;
    // }
#ifdef DEBUG
    std::cerr << "---------- CGI path -----------" << std::endl;
    std::cerr << cgi_path << std::endl;
    std::cerr << "------------  ------------" << std::endl;
#endif

    status = 404;

    struct stat stats;
    if (stat(cgi_path.c_str(), &stats) == 0)
    {
        if (S_ISREG(stats.st_mode)) {
            status = 200;
        }
    }

    if (status/100 == 2) {
        pipe(pip);
    
        pid = fork();
        if (pid == -1) {
            perror("fork failed: ");
            return ;
        } else if (pid == 0) {
#ifdef DEBUG
            std::cerr << "---- child ----" << std::endl;
#endif
            close(pip[1]); //close fd write pipe
            dup2(pip[0], 0); //redirect pipe read to 0
            close(pip[0]); // close pipe read

            char    *args[2];
            args[0] = (char *)cgi_path.c_str();
#ifdef DEBUG
            std::cerr << "path = " << args[0] << std::endl;
#endif
            args[1] = NULL;

            char    **env = get_cgi_env();

#ifdef DEBUG
            std::cerr << "---- envs ----" << std::endl;
            for (int i = 0; env[i] != NULL; ++i){
                std::cerr << "env = " << env[i] << std::endl;
            }
            std::cerr << "---- gonna excve ----" << std::endl;
#endif
            int fd = open("/tmp/www/test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
            if (fd < 0){
#ifdef DEBUG
                std::cerr << "ERROR" << std::endl;
#endif
            }else{
                dup2(fd, 1);
                dup2(fd, 2);
                chdir("/tmp/cgi-bin/");
                execve(args[0], args, env);
            }
            perror("execve failed: ");
            exit(-1);
        } else {
            close(pip[0]); //close reading pipe
            write(pip[1], this->query.body.c_str(), this->query.body.size());
            close(pip[1]); //close wrting pipe

            std::string request;
            int child_status;
#ifdef DEBUG
            std::cerr << "---- wait child to end ----" << std::endl;
#endif
            wait(&child_status);
#ifdef DEBUG
            std::cerr << "---- child ended ----" << std::endl;
#endif
            if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == -1){
                this->content = "";
                return ;
            } 
            this->content = file_to_string("/tmp/www/test.txt"); //read file 
            unlink("/tmp/www/test.txt"); //delete file
            std::vector<string> ttt = split(content, "\r\n");
#ifdef DEBUG
            std::cerr << ttt.at(ttt.size() - 1).size() << std::endl;
#endif

            header.addHeader("Content-Length", std::to_string(ttt.at(ttt.size() - 1).size()));
#ifdef DEBUG
            std::cerr << "---- end ----" << std::endl;
#endif
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
        if (!query.path.compare(0, dir.size(), dir) && dir.size() >= compatibility)
        {
            loc = i;
            compatibility = dir.size();
        }
    }
    query.path = query.path.substr(server.locations[loc].dir.size() - 1);
    if (query.path[0] == '/') query.path = query.path.substr(1);
}

string  Response::render() {
    std::vector<std::string> allImplementedMethods = implementedMethods();

    if (!std::count(allImplementedMethods.begin(), allImplementedMethods.end(), query.command))
        error(501); // not implemented

    else if (!std::count(server.locations[loc].methods.begin(), server.locations[loc].methods.end(), query.command))
        error(405); // not authorized

    else
        execute();

    std::string response(statusLine(status) + header.toString() + content + "\n");

#ifdef DEBUG
    std::cerr << "_____RESPONSE_____ [" << response.size() << "]" << std::endl;
    std::cerr << response.substr(0, 1000);
    std::cerr << "_____        _____" << std::endl;
#endif
    return response;
}
