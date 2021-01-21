#ifndef DATE_HPP
# define DATE_HPP
# include <ctime>
# include <iostream>

std::string string_date();
std::tm* tm_date(const char* format_time);

#endif
