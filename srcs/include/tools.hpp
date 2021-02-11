#ifndef TOOLS_HPP
# define TOOLS_HPP
# define DIRECTORY_STATS 040000
# include <vector>

// DATE
std::string string_date();
std::string string_date(struct tm*  time);
std::string index_date(struct tm*  time);
struct tm* tm_date(const char* format_time);

// FILES
std::string file_to_string(std::string path);
bool        is_dir(std::string path);
void        create_file(std::string path, std::string content);
void        mkdir_p(std::string path);

// NUMBERS
size_t width(int n);

// STRING UTILS
std::vector<std::string> split(std::string s, std::string delimiter);
std::string trim(std::string str);
std::string trim(std::string str, std::string tr);

// POST UTILS
char    **get_child_env(std::string &query_string);

//hotns forbiden function
uint16_t ft_htons(uint16_t tmp);

#endif
