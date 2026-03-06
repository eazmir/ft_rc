#include "../include/client.hpp"

managerchannel::managerchannel(std::map<int, client> &clients,const std::string &pass)
    : _clients(clients),
    _password(pass)
{
}

void managerchannel::handle_input(const std::string &input, client &c)
{
    if (input.compare(0,4,"PING") == 0)
    {
        const char *line = "----------------------------------------\n";
        const char *msg = "PONG\r\n";
        send(c.fd,line,strlen(line),0);
        send(c.fd,msg,strlen(msg),0);
        send(c.fd,line,strlen(line),0);
    }
    // Test How to send to all clinet in network


    //For example :
    if (input.compare(0,4,"test") == 0)
    {
        std::map<int ,client>::iterator it;
        for (it = _clients.begin(); it != _clients.end();it++)
        {
            if (it->first  != c.fd)
            {
                const char *line = "----------------------------------------\n";
                size_t pos = input.find(":");
                if (pos != std::string::npos)
                {
                    std::string msg = input.substr(pos + 1) + "\r\n";
                    send(it->first,line,strlen(line),0);
                    send(it->first,msg.c_str(),strlen(msg.c_str()),0);
                    send(it->first,line,strlen(line),0);
                }
            }
        }
    }
}