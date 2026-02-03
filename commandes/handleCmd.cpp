# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include <string>


/*
    ---------------------------------------
    | Command   |  Notes                  |
    | --------- | ------------------------|
    | `PING`    | Must respond with PONG  |
    | `PONG`    | Minimal handling ok     |
    | `PRIVMSG` | User→user first         |
    | `NOTICE`  |Same parsing as PRIVMSG  |
    | `WHOIS`   | Minimal version ok      |
    | `NICK`    | Nick change             |
    | `QUIT`    |Always allowed           |
    ---------------------------------------
*/

void Server::_handleCmd(Client* client, std::string& fullCmd ,std::vector<std::string>& tokens)
{
    std::string cmd = tokens[0];

    if (client->checkAuthComplete())
    {
        if (cmd == "JOIN")
        {
            _cmdJoin(client, tokens); return;
        }
        else if (cmd == "TOPIC")
        {
            _cmdTopic(client, tokens); return;
        }
        else if (cmd == "PRIVMSG")
        {   
            _handlePrivmsg(client, fullCmd); return;
        }
        else if (cmd == "PONG")
        {
            // _cmdPong(client, tokens); return; // TODO
        }
        // ...
    }
}
