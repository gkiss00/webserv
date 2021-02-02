#include "webserv.hpp"

std::string file_to_string(std::string path) {
    std::string content = "";
    int ret;
    char buf[257];

    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1)
        return content;
    while ((ret = read(fd, buf, 256)))
    {
        buf[ret] = '\0';
        content += buf;
    }
    close(fd);
    return content;
}

bool        is_dir(std::string path) {
    struct stat stats;

    if (stat(path.c_str(), &stats) == 0)
    {
        if (stats.st_mode & DIRECTORY_STATS)
            return true;
    }
    return false;
}

void        create_file(std::string path, std::string content) {
    std::cout << path << std::endl;
    int fd = open(path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd == -1)
        return ;
    write(fd, content.c_str(), content.size());
    close(fd);
}

void        mkdir_p(std::string path) {
    struct stat stats;

    if (stat(path.c_str(), &stats) == 0)
    {
        if (stats.st_mode & DIRECTORY_STATS)
            return ;
    }
    mkdir(path.c_str(), 0777);
}
