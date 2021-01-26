#include "webserv.hpp"

std::string file_to_string(std::string path) {
    std::ifstream file(path);
    std::stringstream buffer;

    buffer << file.rdbuf();
    return buffer.str();
}
