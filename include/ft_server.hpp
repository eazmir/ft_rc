#include <iostream>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <poll.h>
#include <vector>
#include <algorithm>
#include <string>

struct sockaddr_in2 
{
    short            sin_family;   // e.g. AF_INET (IPv4)
    unsigned short   sin_port;     // Port number (must use htons())
    struct in_addr   sin_addr;     // IP address (usually INADDR_ANY)
    char             sin_zero[8];  // Padding to make it the same size as sockaddr
};

struct client
{
    int fd;
    std::string username;
};

struct poll_in
{
    int n;
    std::vector<struct pollfd>fds;
};


class Server
{
    private:
        std::vector<client> clients; 
        std::string username;
        std::string message;
        std::string clinet;
        std::string password;
        int index;
        bool pass_ok;
        bool user_ok;
        int registered;
    public:
        Server(int _port,std::string passwod);
        int ft_multiplexing(int &fd_Server,std::string password);
        int ft_message(std::string &str);
        int ft_username(std::string &str);
        std::vector<struct pollfd> ft_init_poll(int &fd);
        void ft_client(int newClinet,poll_in *p);
        void ft_breadcast(int sender,std::string &str);
        void ft_password(std::string &str);
};