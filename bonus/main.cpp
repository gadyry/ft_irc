
# include "../includes/Bot.hpp"

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

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

int main(int ac, char** av)
{
    if (ac != 3)
    {
        LOG(ERROR,  "Usage: ./ircserv <port> <password>");
        return (1);
    }
    if (!validPort(av[1]) || !av[2][0])
    {
        LOG(ERROR, "Error: " << "try again: ./ircserv <port> <password>");
        return (1);
    }
    // try
    // {
    //     Bot bot(std::strtol(av[1], NULL, 10), av[2]);
    //     bot.executeBot();
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << e.what() << std::endl;
    //     return (1);
    // }
    return (0);
}