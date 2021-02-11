#include "webserv.hpp"

std::string file_to_string(std::string path) {
    std::string content = "";
    int ret;
    char buf[256];

    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1){
        // std::cerr << "open RDONLY error" << std::endl;
        return content;
    }
    while ((ret = read(fd, buf, 255)) > 0)
    {
        buf[ret] = '\0';
        content += buf;
    }
    if (ret == -1)
        std::cerr << "read file_to_string error" << std::endl;
    close(fd);
    return content;
}

bool        is_dir(std::string path) {
    struct stat stats;

    if (stat(path.c_str(), &stats) == 0) {
        if (stats.st_mode & DIRECTORY_STATS)
            return true;
    }
    return false;
}

void        create_file(std::string path, std::string content) {
    int fd = open(path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 666);
    if (fd == -1)
        return ;
    int ret;
    std::string copy(content);
    while ((ret = write(fd, copy.c_str(), copy.size())) > 0) {
        if (static_cast<unsigned int>(ret) >= copy.size())
            break ;
        copy = copy.substr(ret);
    }
    if (ret == -1) std::cerr << "write failed" << std::endl;
    close(fd);
}

void        mkdir_p(std::string path) {
    struct stat stats;

    if (stat(path.c_str(), &stats) == 0) {
        if (stats.st_mode & DIRECTORY_STATS)
            return ;
    }
    mkdir(path.c_str(), 0777);
}
