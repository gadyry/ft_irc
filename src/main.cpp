/*
    => Read RFC 1459 (only sections about commands:
            NICK, USER, JOIN, PRIVMSG, PART, QUIT, etc.)

    => Create a minimal Server class that binds and listens on a port.
*/
#include <iostream>
// #include "Server.hpp"
// #include "Client.hpp"
#include <cstdlib>
#include <cstdio>

bool validPort(char* str)
{
    int i = 0;
    while (str[i])
    {
        if (!std::isdigit(str[i]))
            return (false);
        i++;
    }

    return (true);
}

int main(int ac, char** av)
{
    if (ac != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (1);
    }

    if (!validPort(av[1]))
    {
        std::cerr << "Invlaid port, try again: ./ircserv <port> <password>\n";
        return (1);
    }

    // Server serv;
    // serv.port = std::atoi(av[1]);
    // serv.password = av[2];

    return (0);
}