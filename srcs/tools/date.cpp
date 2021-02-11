#include <iostream> // just for the strings
#include <sys/time.h>

// result: "Fri, 31 Dec 1999 23:59:59 GMT"

std::string string_date(struct tm*  time) {
    char        buf[64];

    strftime (buf, 64, "%a, %d %b %Y %T GMT", time);
    return std::string(buf);
}

std::string string_date() {
    struct timeval t;
    gettimeofday(&t, NULL);
    struct tm *   now = localtime(&t.tv_sec);

    return string_date(now);
}

std::string index_date(struct tm*  time) {
    char        buf[64];

    strftime (buf, 64, "%d-%b-%Y %R", time);
    return std::string(buf);
}

// from "Fri, 31 Dec 1999 23:59:59 GMT" to struct tm

struct tm* tm_date(const char* format_time) {
    struct timeval t;
    gettimeofday(&t, NULL);
    struct tm *   time = localtime(&t.tv_sec);

    strptime(format_time, "%a, %d %b %Y %T GMT", time);
    return time;
}
