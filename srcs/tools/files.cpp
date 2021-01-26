#include "webserv.hpp"

std::string file_to_string(std::string path) {
    std::string content;
    int ret;
    char buf[257];

    int fd = open(path.c_str(), O_RDONLY);
    while ((ret = read(fd, buf, 256)))
    {
        buf[ret] = '\0';
        content += buf;
    }
    close(fd);
    return content;
}
