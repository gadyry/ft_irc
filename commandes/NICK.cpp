# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"

void    Server::_cmdNick(Client* client, std::vector<std::string>& tokens)
{
    if (tokens.empty()) return;

    if (tokens.size() < 2)
    {
        sendError(client, "CHEE 9LWA => :IRCServer 461 NICK :Not enough parameters");
        return;
    }
    if (tokens.size() > 2)
    {
        sendError(client, ERR_ERRONEUSNICKNAME(client->getNickname()));
        return;
    }
    if (false)
    {
        // TODO : Handle somthing!!!
    }
}
