# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"

/*
    =============== We don't need to handle it ===============
    ERR_NICKCOLLISION – “Returned by the server to a client when the client’s desired 
    nickname is temporarily unavailable because of a nick collision.”
    8=> This situation only happens in an IRC network with multiple servers.
*/

bool    existNickname(std::string& nickname)
{
    // TODO: check If nickname is already in server nickname map
}

bool    nickIsValid(std::string& nickname)
{
    if (nickname.length() > LIMIT_NICK)
        return (false);

    std::string allowedSymbols = "-[]\\^{}|";

    if (!isalpha(nickname[0]) || allowedSymbols.find(nickname[0]) != std::string::npos)
        return (false);

    for(size_t i = 1; i < nickname.length(); i++)
    {
        if (!isalpha(nickname[i]) || !isdigit(nickname[i])
                || allowedSymbols.find(nickname[i]) != std::string::npos)
            return (false);
    }
    return (true);
}

void    Server::_cmdNick(Client* client, std::vector<std::string>& tokens)
{
    if (tokens.empty()) return;

    if (tokens.size() < 2)
    {
        sendError(client, ERR_NONICKNAMEGIVEN(client->getNickname()));
        return;
    }

    std::string& nickname = tokens[1];

    if (!nickIsValid(nickname))
    {
        sendError(client, ERR_ERRONEUSNICKNAME(client->getNickname(), nickname));// Badnick WTF ??
        return;
    }

    if (existNickname(nickname))
    {
        sendError(client, ERR_NICKNAMEINUSE(client->getNickname(), nickname));
        return;
    }

    // The next step I should handle all cases when this shit successful!

}