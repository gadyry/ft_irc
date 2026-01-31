# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"

void    Server::_cmdNick(Client* client, std::vector<std::string>& tokens)
{
    /*
        1. Missing nickname? → error
        2. Invalid characters? → error
        3. Already in use? → error
        4. OK? → update client.nick
        5. Check if client now fully registered
    */
    if (tokens.empty()) return;

    if (client->checkAuthComplete())
    {
        sendError(client, ERR_ALREADYREGISTRED(client->getNickname()));
        return;
    }
    if (tokens.size() < 2)
    {
        sendError(client, "Cheee 9lwa tanrjee3 liha !!");
        return;
    }
    if (tokens.size() > 2)
    {
        sendError(client, ERR_ERRONEUSNICKNAME(client->getNickname()));
        return;
    }

}
