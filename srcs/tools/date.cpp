#include <sstream>
#include "date.hpp"

// result: "Fri, 31 Dec 1999 23:59:59 GMT"

std::string string_date() {
    char        buf[64];
    std::time_t t = std::time(0);
    std::tm *   now = std::localtime(&t);
    
    strftime (buf, 64, "%a, %d %b %Y %T GMT", now);
    return std::string(buf);
}

// from "Fri, 31 Dec 1999 23:59:59 GMT" to struct tm

std::tm* tm_date(const char* format_time) {
    std::time_t t = std::time(0);
    std::tm *   time = std::localtime(&t);

    strptime(format_time, "%a, %d %b %Y %T GMT", time);
    return time;
}
