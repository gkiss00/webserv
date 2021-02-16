#include "webserv.hpp"

MyWebServer webserv;

int	g_thread_pool_size = 1;

void signal_handler(int signal){
    std::cout << "Caught signal: " << signal << std::endl;
    if (signal == SIGINT)
        exit(0);
    // webserv.run();
}

int main()
{
    // signal(SIGPIPE, signal_handler);
    // signal(EPIPE, signal_handler);
    signal(SIGINT, signal_handler); 
    webserv.run();
}