#include "../include/ft_server.hpp"

#define MAX_CLIENT 3



// void Server::parse_msg(char *buffer,int bytes,Client client)
// {
//     buffer[bytes] = '\0';
//     std::string str(buffer);
//     size_t pos_priv = str.find("PRIVMSG");
//     if (pos_priv != std::string::npos)
//     {
//         size_t colon = str.find(':', pos_priv);
//         if (colon != std::string::npos)
//         {
//             std::string user = str.substr(pos_priv + 8, colon - (pos_priv + 8)); 
//             size_t end = user.find_first_of("\r\n");
//             if (end != std::string::npos)
//                 this->username = user.substr(0, end);
//             size_t msg_start = str.find(':');
//             if (msg_start != std::string::npos)
//             {
//                 this->message = str.substr(msg_start + 1);
//                 std::vector<std::string>::iterator it;
//                 it = std::find(client.username.begin(), client.username.end(), user);
//                 if (it != client.username.end())
//                     this->index = std::distance(client.username.begin(),it);
//                 else
//                     std::cout<<"user is not found"<<std::endl;
//             }
//         }
//     }
// }

int Server::parse_user(std::string &str)
{
    size_t pos = str.find(':');
    if (pos != std::string::npos)
    {
        std::string user = str.substr(pos + 1);
        size_t end = user.find_first_of(" \r\n\t");
        if (end != std::string::npos)
        {
            this->clinet = user.substr(0,end);
            return(1);
        }
    }
    return (0);
}

int Server::parse_msg(std::string &str)
{
    size_t pos_priv = str.find("PRIVMSG");
    if (pos_priv == std::string::npos)
        return (0);
        
    size_t colon = str.find(':', pos_priv);
    if (colon == std::string::npos)
        return (0);
        
    std::string user = str.substr(pos_priv + 8, colon - (pos_priv + 8));
    size_t end = user.find_first_of(" \r\n\t");
    if (end != std::string::npos)
        user = user.substr(0, end);
        
    this->username = user;
    this->message = str.substr(colon + 1);
    std::vector<client>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); ++it)
    {
        if (it->username == user)
            break;
    }
    if (it != this->clients.end())
    {
        this->index = std::distance(this->clients.begin(), it);
        std::cout << "Message to: " << user << " (index: " << this->index << ")" << std::endl;
    }
    else
    {
        std::cout << "User '" << user << "' not found" << std::endl;
        this->index = -1;
    }
    return (1);
}


int Server::ft_multiplixing(int &fd_Server)
{
    std::vector<struct pollfd> fds;
    struct pollfd server_fd;

    server_fd.fd = fd_Server;
    server_fd.events = POLLIN;
    fds.push_back(server_fd);
    
    int nums_fd = 1;
   while (true)
   {
        poll(&fds[0],nums_fd,-1);
        for (int i = 0; i < nums_fd;i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == fd_Server)
                {
                    int new_client = accept(fd_Server,NULL,NULL);
                    struct pollfd new_poll;
                    new_poll.fd = new_client;
                    new_poll.events = POLLIN;
                    fds.push_back(new_poll);
                    std::cout << "New client connected: fd = " << new_client << std::endl;
                    nums_fd++;
                }
                else
                {
                    char buffer[1024];
                    int bytes = recv(fds[i].fd,buffer,sizeof(buffer)-1,0);
                    buffer[bytes] = '\0';
                    std::string str(buffer);
                    if (this->parse_user(str))
                    {
                        client newclient;
                        newclient.fd = fds[i].fd;
                        newclient.username = this->clinet;
                        clients.push_back(newclient);
                    }
                    if (this->parse_msg(str))
                        send(clients[this->index].fd,this->message.c_str(),bytes,0);
                }
            }
        }
   }
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
    ft_multiplixing(_socket_fd);
    close(_socket_fd);
}