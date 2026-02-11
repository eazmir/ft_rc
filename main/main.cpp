#include "../include/ft_server.hpp"

int main(int argc,char *argv[])
{
    if (argc != 2)
        return (0);
    Server Server(std::atoi(argv[1]));
}