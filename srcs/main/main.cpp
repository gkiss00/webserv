#include "webserv.hpp"

#define CONFIG_FILE_PTAH "srcs/config/new.conf"

//GLOBAL VARS
fd_set              current_sockets; //list of all sockets
std::vector<Server> servers;
int                 max;

void    get_server_list(){
    NewConfigFileReader cfr;
    servers = cfr.read(CONFIG_FILE_PTAH);
    for (unsigned int i = 0; i < servers.size(); ++i){
        servers.at(i).print();
    }
}

//CONFIGURE NON BLOCKING
int     configure_non_bloking(int fd){
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
        perror("We have got a socket problem, quitting: ");
        return (-1);
    }
    //fill the struct the server socket will refere to
    addrServer.sin_addr.s_addr = inet_addr(host.c_str()); //HOST
    addrServer.sin_family = AF_INET; //IPV4
    addrServer.sin_port = htons(port); //PORT
    //bind the server socket with the struct
    if (bind(server_socket, (const struct sockaddr *)&addrServer, sizeof(addrServer)) == -1)
    {
        perror("Bind failed, quitting: ");
        close(server_socket);
        return (-1);
    }
#ifdef DEBUG
    std::cout << "Bind #" << server_socket <<  " succeed." << std::endl;
#endif
    if (configure_non_bloking(server_socket) == -1){
        perror("Non blocking failed, quitting: ");
        close(server_socket);
        return (-1);
    }
    return (server_socket);
}

//IS THE SOCKET PART FROM THE SERVER SOCKET
bool    isServerSocket(int fd){
    for (unsigned int i = 0; i < servers.size(); ++i){
        if (fd == servers.at(i).socket)
            return (true);
    }
    return (false);
}

//ACCEPT THE CONNECTION
int     accept_new_connexion(int server_socket)
{
    int                 client_socket;
    struct sockaddr_in  client_address;
    socklen_t           csize = sizeof(client_address);

    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &csize)) == -1)
    {
        std::cout << "Connexion with client failed." << std::endl;
        return (-1);
    }
#ifdef DEBUG
    std::cout << "Connexion with client succeed." << std::endl;
#endif
    return (client_socket);
}

//ADD THE NEW CLIENT IN FD_SET
int     add_new_client(int server_socket)
{
    int client_socket;

    //get the new connexion
    if ((client_socket = accept_new_connexion(server_socket)) == -1)
    {
        std::cout << "Could not add a new client." << std::endl;
        return -1;
    }
#ifdef DEBUG
    std::cout << "Client #" << client_socket <<  " has been added to the server." << std::endl;
#endif
    FD_SET(client_socket, &current_sockets);
    if(client_socket > max)
        max = client_socket + 1;
    //add the client to the good server
    for (unsigned int i = 0; i < servers.size(); ++i){
        if (servers.at(i).socket == server_socket)
            servers.at(i).client_sockets.push_back(client_socket);
    }
    return (client_socket);
}

//GET THE REQUEST FROM THE CLIENT
std::string     get_client_request(int client_socket){
    std::string request;
    char        buf[1000001];
    int         ret;

    while ((ret = recv(client_socket, buf, 1000000, 0)) > 0){
        buf[ret] = '\0';
        request += buf;
        usleep(100);
    }
    // if (ret == -1){
    //     throw request_exception("recv fail", 500);
    // }
    return (request);
}

//SEND THE RESPONSE TO THE CLIENT
void        send_client_response(int client_socket, std::string response){
    while(response != ""){
        int ret = send(client_socket, response.c_str(), response.size(), 0);
        if (ret < 0){
#ifdef DEBUG
            std::cout << "bad bad bad" << std::endl;
#endif
        }else if (ret != (int)response.size()){
            response = response.substr(ret, response.size() - ret);
#ifdef DEBUG
            std::cout << "MHHHHHHHH " << ret << std::endl;
#endif
        }else{
#ifdef DEBUG
            std::cout << "good good good " << std::endl;
#endif
            break;
        }
    }
}

void        close_client_socket(unsigned int client_socket) {
    for(unsigned int i = 0; i < servers.size(); i++)
    {
        for (unsigned int j = 0; j < servers[i].client_sockets.size(); ++j)
        {
            if (servers[i].client_sockets[j] == static_cast<int>(client_socket))
            {
                servers[i].client_sockets.erase(servers[i].client_sockets.begin() + j);
            }
        }
    }
    FD_CLR(client_socket, &current_sockets);
    close(client_socket);
}

void signal_handler(int signal){ 
    std::cout << "Caught signal: " << signal << std::endl;
    std::cout << "Exiting..." << std::endl;
    FD_ZERO(&current_sockets);
    for (unsigned int i = 0; i < servers.size(); ++i){
        close(servers.at(i).socket);
    }
    exit(-1);
}

void    set_server_sockets(){
    for (unsigned int i = 0; i < servers.size(); ++i){
        servers.at(i).socket = init_server_socket("127.0.0.1", servers.at(i).listen);
        if(servers.at(i).socket == -1) {
            std::cout << "Error while init server socket" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

Server &get_good_server(unsigned int client_socket)
{
    for(unsigned int i = 0; i < servers.size(); i++)
    {
        for (unsigned int j = 0; j < servers[i].client_sockets.size(); ++j)
        {
            if (servers[i].client_sockets[j] == static_cast<int>(client_socket))
            {
                return(servers[i]);
            }
        }
    }
    return(servers[0]);
}

//ENTRY POINT
int     main(){
    fd_set  copy;
    int     socket_count, compteur_de_requetes = 0;

    signal(SIGINT, signal_handler); 
    //read the config file
    get_server_list();
    //set all serverSocket
    set_server_sockets();
    //configure max
    max = servers.back().socket + 1;
    //configure fd_set
    FD_ZERO(&current_sockets);
    for (unsigned int i = 0; i < servers.size(); ++i){
        FD_SET(servers.at(i).socket, &current_sockets);
    }
    //start listening
    for (unsigned int i = 0; i < servers.size(); ++i){
        listen(servers.at(i).socket, 0);
    }
    //start server
    while(true){
        //copy the fd_set
        copy = current_sockets;
        //select
        if((socket_count = select(max + 1, &copy, NULL, NULL, NULL)) == -1){
#ifdef DEBUG
            std::cout << "Select error, quitting." << std::endl;
#endif
        }else{
            //for each fd
            for (int i = 0; i <= max; ++i){
                //if the fd is in the set
                if (FD_ISSET(i, &copy)){
                    //accept new connexion
                    if (isServerSocket(i)){
#ifdef DEBUG
                        std::cout << "new connexion" << std::endl;
#endif
                        //accept
                        add_new_client(i);
                    //get action from client
                    }else{
                        std::cout << "[" << compteur_de_requetes++ << "]" << std::endl;
                        //read
                        try{
                            RequestParser   request(get_client_request(i));

                            if (compteur_de_requetes >= 98000){
                                std::cout << "__________client_request__________" << std::endl;
                                std::cout << "Command = " << request.command << std::endl;
                                std::cout << "Path = " << request.path << std::endl;
                                std::cout << "HTTP_version = " << request.HTTP_version << std::endl;
                                for (std::map<std::string, std::string>::iterator it = request.headers.begin(); it != request.headers.end(); ++it){
                                    std::cout << "Headers = " << it->first << " : " << it->second << std::endl;
                                }
                                std::cout << "Body[" << request.body.size() << "] = " << request.body.substr(0, 1000) << std::endl;
                                
                                //write
                                std::cout << "__________server_print__________" << std::endl;
                            }

                            Response response(request, get_good_server(i)); // need to take care of all servers, but for now we focus on the 5000
                            send_client_response(i, response.render());
                        }catch(request_exception &e){
                            // bad message
                            std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
                            send_client_response(i, "HTTP/1.1 " + std::to_string(e.get_error_status()) + " " + statusCodes()[e.get_error_status()] + "\n");
                        }
                        
                        //close the socket
                        close_client_socket(i);
                    }
                }
            }
        }
    }
    return (EXIT_SUCCESS);
}