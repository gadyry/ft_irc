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
    if (!str || !str[0])
        return (false);

    for (int i = 0; str[i]; i++)
        if (!std::isdigit(str[i]))
            return (false);

    u_short port = std::atoi(str);
    return (port > 0 && port <= 65535); // 16 bits → 2^16=65536 possible values
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
        std::cerr << "Error: invalid port, "
                  << "try again: ./ircserv <port> <password>\n";
        return (1);
    }

    Server serv(std::atoi(av[1]), av[2]);
    return (0);
}