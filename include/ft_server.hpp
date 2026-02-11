#include <iostream>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>

struct sockaddr_in2 
{
    short            sin_family;   // e.g. AF_INET (IPv4)
    unsigned short   sin_port;     // Port number (must use htons())
    struct in_addr   sin_addr;     // IP address (usually INADDR_ANY)
    char             sin_zero[8];  // Padding to make it the same size as sockaddr
};

struct pollfds
{
    int fds;
    short events;
    short reevnts;
};


class Server
{
    private:
        // int short port;
    public:
        Server(int _port);
        int ft_multiplixing(int fd);
};