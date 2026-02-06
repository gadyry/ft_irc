
# include "../includes/MovieBot.hpp"

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
    if (!validPort(av[1]) || !validPass(av[2]))
    {
        LOG(ERROR, "Error: " << "try again: ./ircserv <port> <password>");
        return (1);
    }
    try
    {
        MovieBot bot(std::strtol(av[1], NULL, 10), av[2]);
        bot.executeMovieBot();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}