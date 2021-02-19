#include "./Proxy.hpp"

Proxy::Proxy(){
    this->addr = "127.0.0.1";
    this->port = 5007;
}

Proxy::~Proxy(){}

std::string Proxy::getResponse(std::string request) {
    std::cout << "PROXY CALLED" << std::endl;
    int     socketClient = socket(AF_INET, SOCK_STREAM, 0);
    std::string response;
    struct sockaddr_in addrClient;
    char buff[100001];

    addrClient.sin_addr.s_addr = inet_addr(this->addr.c_str()); // should be variable
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = ft_htons(this->port);

    if (connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient)) < 0)
        return "";
    if (send(socketClient, request.c_str(), request.size() + 1, 0) < 0){
        close(socketClient);
        return "";
    }
    int ret = recv(socketClient, buff, 10000, 0);
    if (ret < 0){
        close(socketClient);
        return "";
    }
    buff[ret] = '\0';
    response += buff;
    close(socketClient);
    return response;
}