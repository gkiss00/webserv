

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

# include <iostream>
# include <vector>
# include <list>
# include <queue>
# include <stack>
# include <map>
# include <algorithm>

# include <iomanip>
# include <fstream>
# include <sstream>

# include "../RequestParser/RequestParser.hpp"
# include "tools.hpp"

class RequestParser;
std::string response(RequestParser query);

#endif