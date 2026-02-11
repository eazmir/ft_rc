#include "../include/ft_server.hpp"

int Server::ft_multiplixing(int fd)
{
    
}

Server::Server(int _port)
{
    sockaddr_in2 sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(_port);

    int _socket_fd = socket(AF_INET, SOCK_STREAM,0);
    bind(_socket_fd,(struct sockaddr*)&sockaddr,sizeof(sockaddr));

    listen(_socket_fd,10);

    close(_socket_fd);
}