#include "./Proxy.hpp"

Proxy::Proxy(){
    this->addr = "127.0.0.1";
    this->port = 50000;
}

Proxy::~Proxy(){}

std::string Proxy::getResponse(std::string request) {
    int     socketClient = socket(AF_INET, SOCK_STREAM, 0);
    std::string response;
    struct sockaddr_in addrClient;
    char buff[100001];

    addrClient.sin_addr.s_addr = inet_addr(this->addr.c_str()); // should be variable
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = ft_htons(this->port);

    connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
    send(socketClient, request.c_str(), request.size() + 1, 0);
    int ret = recv(socketClient, buff, 10000, 0);
    buff[ret] = '\0';
    response += buff;

    return response;
}