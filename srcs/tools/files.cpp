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