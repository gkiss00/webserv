#ifndef TOOLS_HPP
# define TOOLS_HPP
# include <ctime>
# include <iostream>

// DATE
std::string string_date();
std::tm* tm_date(const char* format_time);

// FILE_TO_STRING
std::string file_to_string(std::string path);

#endif
