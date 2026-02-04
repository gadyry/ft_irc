# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"


/*
==================================================
| Condition             | Error                  |
| --------------------- | ---------------------- |
| No target             | `ERR_NORECIPIENT`      |
| No message            | `ERR_NOTEXTTOSEND`     |
| Channel doesn't exist | `ERR_NOSUCHCHANNEL`    |
| Not member of channel | `ERR_CANNOTSENDTOCHAN` |
| Nick doesn't exist    | `ERR_NOSUCHNICK`       |
==================================================
*/
void    Server::_handlePrivmsg(Client* senderCl, std::vector<std::string>& tokens)
{
    std::string target = tokens[1];
    std::string word = tokens[2];
    std::string msg, fullmsg;

    if (tokens.size() < 2)
    {
        sendError(senderCl, ERR_NORECIPIENT(senderCl->getNickname(), "PRIVMSG"));
        return;
    }
    if (tokens.size() < 3 || word[0] != ':' || word == ":")
    {
        sendError(senderCl, ERR_NOTEXTTOSEND(senderCl->getNickname()));
        return;
    }

    // extract full msg from tokens[2] ... tokens[size - 1]
    for(size_t i = 2; i < tokens.size(); i++)
    {
        if (i == 2)
            msg += tokens[2].substr(1);
        else
            msg += " " + tokens[i];
    }

    /* 
        8=============>
        Grammer of full msg : :nick!user@host PRIVMSG <receiver> :<text>
        [':' <prefix> <SPACE> ] <command> <params> <CRLF> 
        2.3.1 Message format in 'pseudo' BNF
    */
    fullmsg = ":" + senderCl->getNickname() + "!"
               + senderCl->getUsername() + "@"
               + senderCl->getHost() + "PRIVMSG"
               + target + msg + "\r\n";
    
    // ====> channel usage:
    if (target[0] == '#')
    {
        Channel* channel = getChannel(target);
        if (!channel)
        {
            sendError(senderCl, ERR_NOSUCHCHANNEL(senderCl->getNickname(), target)); return;
        }
        if (!channel->isMember(senderCl))
        {
            sendError(senderCl, ERR_CANNOTSENDTOCHAN(senderCl->getNickname(), target));
            return;
        }
        // broadcast !!
    }
}