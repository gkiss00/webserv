#include <sstream>
#include "tools.hpp"

// result: "Fri, 31 Dec 1999 23:59:59 GMT"

std::string string_date() {
    std::time_t t = std::time(0);
    std::tm *   now = std::localtime(&t);

    return string_date(now);
}

std::string string_date(std::tm*  time) {
    char        buf[64];

    strftime (buf, 64, "%a, %d %b %Y %T GMT", time);
    return std::string(buf);
}

std::string index_date(std::tm*  time) {
    char        buf[64];

    strftime (buf, 64, "%d-%b-%Y %R", time);
    return std::string(buf);
}

// from "Fri, 31 Dec 1999 23:59:59 GMT" to struct tm

std::tm* tm_date(const char* format_time) {
    std::time_t t = std::time(0);
    std::tm *   time = std::localtime(&t);

    strptime(format_time, "%a, %d %b %Y %T GMT", time);
    return time;
}
