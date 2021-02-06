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
# include "../main/MyWebServer.hpp"

// #define DEBUG 1

using std::string;

extern std::vector<Server>  servers;

#endif