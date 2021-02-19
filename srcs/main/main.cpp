#include "webserv.hpp"

MyWebServer webserv;

int	g_thread_pool_size = 1;

void signal_handler(int signal){
    std::cout << "Caught signal: " << signal << std::endl;
    if (signal == SIGINT)
        exit(EXIT_SUCCESS);
}

int main()
{
    signal(SIGINT, signal_handler); 
    webserv.run();
}