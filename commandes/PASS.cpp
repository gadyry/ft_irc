# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"

Server::_cmdPass(Client* client, std::vector<std::string> tokens, std::string& fullCmd)
{
    std::string& cmd = tokens[0];

    if (client->getReg_stat()) // client already regestred!!!
    {
        sendError(client, ERR_ALREADYREGISTRED, "msg Error: client already regestred");
        return;
    }

    if (client->size() < 2) // need more params!!
    {
        sendError(client, ERR_NEEDMOREPARAMS, "msg Error:  need more params");
        return;
    }

    if (client->size() > 2) // need more params!!
    {
        sendError(client, ERR_NEEDMOREPARAMS, "msg Error:  Too many param");
        return;
    }

}
