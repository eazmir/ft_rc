#include "../include/ft_server.hpp"

#define MAX_CLIENT 10

int Server::username(std::string &str)
{
    size_t pos = str.find(':');
    if (pos == std::string::npos)
        return (0);
    std::string user = str.substr(pos + 1);
    size_t end = user.find_first_of(" \r\n\t");
    if (end == std::string::npos)
        return (0);
    this->clinet = user.substr(0, end);
    return (1);
}

int Server::ft_message(std::string &str)
{
    size_t pos_priv = str.find("PRIVMSG");
    if (pos_priv == std::string::npos)
        return (0);
    size_t colon = str.find(':', pos_priv);
    if (colon == std::string::npos)
        return (0);
    std::string user =
        str.substr(pos_priv + 8, colon - (pos_priv + 8));
    size_t end = user.find_first_of(" \r\n\t");
    if (end != std::string::npos)
        user = user.substr(0, end);
    this->username = user;
    this->message = str.substr(colon + 1);
    this->index = -1;
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].username == user)
        {
            this->index = i;
            break;
        }
    }
    if (this->index == -1)
        std::cout << "User '" << user << "' not found\n";
    else
        std::cout << "Message to: " << user
                  << " (index: " << this->index << ")\n";
    return (1);
}

void Server::ft_breadcast(int sender, std::string &str)
{
    size_t pos = str.find(':');
    if (pos == std::string::npos)
        return;
    std::string msg = str.substr(pos + 1);
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].fd == sender)
            continue;
        send(clients[i].fd, msg.c_str(), msg.length(), 0);
    }
}

std::vector<struct pollfd> Server::ft_init_poll(int &fd)
{
    std::vector<struct pollfd> fds;

    struct pollfd server_fd;
    server_fd.fd = fd;
    server_fd.events = POLLIN;
    fds.push_back(server_fd);
    return (fds);
}

void Server::ft_client(int newClient, poll_in *p)
{
    struct pollfd newPoll;
    newPoll.fd = newClient;
    newPoll.events = POLLIN;
    p->fds.push_back(newPoll);
}

void Server::ft_password(std::string &str)
{
    size_t pos = str.find(':');
    if (pos == std::string::npos)
        return;
    size_t end = str.find_first_of(" \r\t\n");
    if (end != std::string::npos)
    {
        this->password = str.substr(0,end);
    }
}

int Server::ft_multiplexing(int &fd_Server,std::string password)
{
   poll_in *p = new poll_in;
   p->fds = ft_init_poll(fd_Server);
   p->n = 1;

   while (true)
   {
        poll(&p->fds[0],p->n,-1);
        for (int i = 0; i < p->n;i++)
        {
            if (p->fds[i].revents & POLLIN)
            {
                if (p->fds[i].fd == fd_Server)
                {
                    int new_client = accept(fd_Server,NULL,NULL);
                    this->ft_client(new_client,p);
                    std::cout << "New client connected: fd = " << new_client << std::endl;
                    p->n++;
                }
                else
                {
                    char buffer[1024];
                    int bytes = recv(p->fds[i].fd,buffer,sizeof(buffer)-1,0);
                    buffer[bytes] = '\0';
                    std::string str(buffer);
                    if (str.find("username") != std::string::npos)
                    {
                        if (this->ft_username(str))
                        {
                            this->user_ok = true;
                        }
                    }
                    if (str.find("password") != std::string::npos)
                    {
                        if (this->password == password)
                        {
                            this->pass_ok = true;
                        }
                    }
                    if (this->pass_ok && this->user_ok)
                    {

                        client newclient;
                        newclient.fd = p->fds[i].fd;
                        newclient.username = this->clinet;
                        clients.push_back(newclient);
                    }
                    else
                    {
                         send(p->fds[i].fd, "464 :Password incorrect\r\n", 27, 0);
                    }
                    if (str.find("#channel") != std::string::npos)
                    {
                        this->ft_breadcast(p->fds[i].fd,str);
                        this->index = -1;
                        continue;
                    }
                    if (str.find("PRIVMSG") != std::string::npos)
                    {
                        if (this->ft_message(str) && this->index != -1)
                        {
                            send(clients[this->index].
                                    fd,this->message.c_str(),
                                    this->message.length(),0);
                        }
                    }
                }
            }
        }
    }
}

Server::Server(int _port,std::string passwod)
{
    sockaddr_in2 sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(_port);

    int _socket_fd = socket(AF_INET, SOCK_STREAM,0);
    bind(_socket_fd,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
    listen(_socket_fd,10);
    ft_multiplexing(_socket_fd,password);
    close(_socket_fd);
}