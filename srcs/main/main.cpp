#include "./../include/webserv.hpp"

//GLOBAL VARS
fd_set              current_sockets; //list of all sockets
std::vector<int>    server_sockets; //list of all server sockets
int                 max;

//CONFIGURE NON BLOCKING
int     configureNonBloking(int fd){
    int     ret;

    ret = fcntl(fd, F_SETFL, O_NONBLOCK);
    return (ret);
}

//RETURN A CONFIGURED SERVER SOCKET
int     init_server_socket(std::string host, int port)
{
    int                 server_socket;
    struct sockaddr_in  addrServer;

    //create the socket in IPV4 in STREAM protocol
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "We have got a socket problem, quitting." << std::endl;
        return (-1);
    }
    //fill the struct the server socket will refere to
    addrServer.sin_addr.s_addr = inet_addr(host.c_str()); //HOST
    addrServer.sin_family = AF_INET; //IPV4
    addrServer.sin_port = htons(port); //PORT
    //bind the server socket with the struct
    if (bind(server_socket, (const struct sockaddr *)&addrServer, sizeof(addrServer)) == -1)
    {
        std::cout << "Bind failed, quitting." << std::endl;
        return (-1);
    }
    std::cout << "Bind #" << server_socket <<  " succeed." << std::endl;
    if (configureNonBloking(server_socket) == -1){
        std::cout << "Non blocking failed, quitting." << std::endl;
        return (-1);
    }
    return (server_socket);
}

bool    isServerSocket(int fd){
    for (unsigned int i = 0; i < server_sockets.size(); ++i){
        if (fd == server_sockets.at(i))
            return (true);
    }
    return (false);
}

//ENTRY POINT
int     main(int argc, char **argv){
    argv = NULL;
    std::cout << argc << std::endl;

    fd_set  copy;
    int     socket_count;
    //read the config file
    //get all serverSocket
    server_sockets.push_back(init_server_socket("127.0.0.1", 5000)); // test
    //configure max
    max = server_sockets.back() + 1;
    //configure fd_set
    FD_ZERO(&current_sockets);
    for (unsigned int i = 0; i < server_sockets.size(); ++i){
        FD_SET(server_sockets.at(i), &current_sockets);
    }
    //start listening
    for (unsigned int i = 0; i < server_sockets.size(); ++i){
        listen(server_sockets.at(i), 0);
    }
    //start server
    while(true){
        //copy the fd_set
        copy = current_sockets;
        //select
        if((socket_count = select(max + 1, &copy, NULL, NULL, NULL)) == -1){
            std::cout << "Select error, quitting." << std::endl;
        }else{
            //for each fd
            for (int i = 0; i < max; ++i){
                //if the fd is in the set
                if (FD_ISSET(i, &copy)){
                    //accept new connexion
                    if (isServerSocket(i)){
                        std::cout << "new connexion" << std::endl;
                    //get action from client
                    }else{
                        std::cout << "new request" << std::endl;
                    }
                }
            }
        }
        //accept
        //read
        //write
        //maybe close
    }
}