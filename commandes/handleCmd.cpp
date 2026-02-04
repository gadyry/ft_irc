# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include <string>

void Server::_handleCmd(Client* client, std::string& fullCmd ,std::vector<std::string>& tokens)
{
    std::string cmd = tokens[0];

    if (client->checkAuthComplete())
    {
        if (cmd == "JOIN")
        {
            _cmdJoin(client, tokens); return;
        }
        // else if (cmd == "TOPIC")
        // {
        //     _cmdTopic(client, tokens); return;
        // }
        else if (cmd == "KICK")
        {
            _cmdKick(client, tokens); return;
        }
        else if (cmd == "PRIVMSG")
        {   
            _handlePrivmsg(client, tokens); return;
        }
        else if (cmd == "PONG")
        {
            // _cmdPong(client, tokens); return; // TODO
        }
        else if (cmd == "MODE")
        {
            _cmdMode(client, tokens); return;
        }
    }
}
