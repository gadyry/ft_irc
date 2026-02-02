# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"

void Server::_cmdPass(Client* client, std::vector<std::string>& tokens)
{
    if (!client->getNickname().empty() ||
        client->checkAuthComplete() || client->getAuthState() != AUTH_DISCONNECTED)
    {
        sendError(client, ERR_ALREADYREGISTRED(client->getNickname()));
        return;
    }

    if (tokens.size() < 2)
    {
        sendError(client, ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"));
        return;
    }

    std::string pass = tokens[1];
    if (!pass.empty() && pass[0] == ':')
        pass.erase(0, 1);

    if (pass != this->password)
    {
        sendError(client, ERR_PASSWDMISMATCH(client->getNickname()));
        return;
    }

    client->setPassOk();
}

