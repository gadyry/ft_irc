# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"

void    Server::_cmdNick(Client* client, std::vector<std::string>& tokens)
{
    if (tokens.empty()) return;

    // TODO : implement join handling Nick!!
}
