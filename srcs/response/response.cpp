#include "Response.hpp"

Response::Response(RequestParser &query, Server &server)
: query(query), server(server) {
    setAllowedMethods();
}

Response::~Response() {}

void        Response::getStatus() {
    struct stat stats;

    if (stat(query.path.c_str(), &stats) == 0)
    {
        status = 200;
    }
    else
    {
        status = 404;
        query.path = server.error_pages[status];
    }
}

void        Response::setAllowedMethods() {
    std::string allowed;
    for (unsigned int i = 0; i < server.methodes.size(); ++i){
        allowed += server.methodes.at(i);
        if (i != server.methodes.size() - 1)
            allowed += ", ";
    }
    header.addHeader("Allowed-Methods", allowed);
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

void        Response::getFile() {
    struct stat stats;

    content = file_to_string(query.path);
    stat(query.path.c_str(), &stats);
    header.addHeader("Content-Length", std::to_string(content.size()));
    header.addHeader("Content-Type", "text/html");
    header.addHeader("Last-Modified", string_date(gmtime(&stats.st_ctime)));
    header.addHeader("Content-Location", query.path);
    header.addHeader("Content-Type", fileExtension()[query.path.substr(query.path.find_last_of(".") + 1)]);
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

            char    arg1[] = "/Users/corentin/Documents/Programmation/19/webserv/srcs/pages/cgi-bin/add.cgi";
            char    *args[2];
            args[0] = arg1;
            args[1] = NULL;

            char env1[] = "REQUEST_METHOD=POST";
            char *env[2];
            env[0] = env1;
            env[1] = NULL;

            execve("/Users/corentin/Documents/Programmation/19/webserv/srcs/pages/cgi-bin/add.cgi", args, env);
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

void        Response::parse() {
    if (query.command == "GET" || query.command == "HEAD") {
        getStatus();
        getFile();
    } else if (query.command == "DELETE") {
        this->getStatus();
        this->moveFile();
        this->content = "";
    } else if (query.command == "POST") {
        // this->getStatus();
        this->status = 200;
        this->execCGI();
    } else {
        this->status = 405; // 501 ?
        this->content = "";
    }
}

string Response::render() {

    parse();

    std::string response(string("HTTP/1.1 ") + std::to_string(status) + " " + statusCodes()[status] + "\n"
        + header.toString());

    if (this->query.command != "POST"){
        response += "\n" + content + "\r\n";
    }else{
        response += content + "\r\n";
    }

    std::cout << "------------- Response final -------------" << std::endl;
    std::cout << response;
    std::cout << "-------------                -------------" << std::endl;
    return response;
}
