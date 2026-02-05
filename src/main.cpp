/*
    => Read RFC (RFC 1459 / 2812) (only sections about commands:
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

    long port = std::strtol(str, NULL, 10);
    return (port >= 1024 && port <= 65535); // 16 bits → 2^16=65536 possible values
}

bool    validPass(char* password)
{
    if (!password || !password[0]) return (false);

    for (size_t i = 0; password[i]; i++)
    {
        if (std::isspace(password[i]) || password[i] == '\t')
            return (false);
    }
    return (true);
}

int main(int ac, char** av)
{
    if (ac != 3)
    {
        LOG(ERROR,  "Usage: ./ircserv <port> <password>");
        return (1);
    }
    if (!validPort(av[1]) || !validPass(av[2]) ) // X : HANDLE PASS WHEN PASS JUST SPACES!!
    {
        LOG(ERROR, "Error: " << "try again: ./ircserv <port> <password>");
        return (1);
    }
    try
    {
        Server server(std::strtol(av[1], NULL, 10), av[2]);
        server.executeServ();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}
