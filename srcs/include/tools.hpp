#ifndef TOOLS_HPP
# define TOOLS_HPP
# define DIRECTORY_STATS 040000

// DATE
std::string string_date();
std::string string_date(std::tm*  time);
std::string index_date(std::tm*  time);
std::tm* tm_date(const char* format_time);

// FILES
std::string file_to_string(std::string path);
bool        is_dir(std::string path);

// NUMBERS
size_t width(int n);

// STRING UTILS
std::vector<std::string> split(std::string s, std::string delimiter);
std::string trim(std::string str);
std::string trim(std::string str, std::string tr);

// POST UTILS
char    **get_child_env(std::string &query_string);

#endif
