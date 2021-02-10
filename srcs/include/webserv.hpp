#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
// # define _XOPEN_SOURCE       /* See feature_test_macros(7) */
# include <time.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <dirent.h>

# include <iostream>
# include <vector>
# include <list>
# include <queue>
# include <stack>
# include <map>
# include <algorithm>

# include "../requestParser/RequestParser.hpp"
# include "../response/Response.hpp"
# include "tools.hpp"
# include "data.hpp"
# include "../configFileReader/ConfigFileReader.hpp"
# include "../configFileReader/NewConfigFileReader.hpp"
# include "../configFileReader/Server.hpp"
# include "../configFileReader/Location.hpp"
# include "../client/Client.hpp"
# include "../main/MyWebServer.hpp"

// #define DEBUG 1

using std::string;

extern std::vector<Server>  servers;

#define PRINT(x) std::cout << x << std::endl;
#define GRN(x) std::cout << "\e[32m" << x << "\e[m" << std::endl;
#define RED(x) std::cout << "\e[31m" << x << "\e[m" << std::endl;
#define YLW(x) std::cout << "\e[33m" << x << "\e[m" << std::endl;
#define BLU(x) std::cout << "\e[34m" << x << "\e[m" << std::endl;

#endif