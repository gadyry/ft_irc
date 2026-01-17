/*
    => Read RFC 1459 (only sections about commands:
            NICK, USER, JOIN, PRIVMSG, PART, QUIT, etc.)

    => Create a minimal Server class that binds and listens on a port.
*/
#include <iostream>
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
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
    return (port >= 1024 && port <= 65535); // 16 bits → 2^16=65536 possible values
}

int main(int ac, char** av)
{
    if (ac != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    if (!validPort(av[1]) || !av[2][0])
    {
        std::cerr << "Error: "
                  << "try again: ./ircserv <port> <password>\n";
        return (1);
    }
    try
    {
        Server serv(std::atoi(av[1]), av[2]);
        // server.executionServ();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}