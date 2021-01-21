#ifndef TOOLS_HPP
# define TOOLS_HPP
# include <ctime>
# include <iostream>

// DATE
std::string string_date();
std::tm* tm_date(const char* format_time);

// FILE_TO_STRING
std::string file_to_string(std::string path);

// STRING UTILS
std::vector<std::string> split(std::string s, std::string delimiter);
std::string trim(std::string str);

#endif
