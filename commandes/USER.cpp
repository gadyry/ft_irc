# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"

void    Server::_cmdUser(Client* client, std::vector<std::string>& tokens)
{
    if (client->checkAuthComplete() || !client->getUsername().empty())
    {
        sendError(client, ERR_ALREADYREGISTRED(client->getNickname()));
        return;
    }
    if (tokens.size() < 5)
    {
        sendError(client, ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
        return;
    }

    std::string& username = tokens[1];
    std::string& realname = tokens[4];

    if (!client->getUsername().empty() && realname[0] == ':')
        realname.erase(0, 1);

    for(size_t i = 5; i < tokens.size(); i++)
        realname += " " + tokens[i];

    client->setUsername(username);
    client->setUserOk();

    // if (client->checkAuthComplete())
    //     completeRegistration(client);

}
