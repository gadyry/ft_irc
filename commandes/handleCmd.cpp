# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include <string>

void Server::_handleCmd(Client* client, std::vector<std::string>& tokens)
{
    std::string cmd = tokens[0];

    if (client->checkAuthComplete())
    {
        if (cmd == "JOIN")
        {
            _cmdJoin(client, tokens);
        }
        else if (cmd == "TOPIC")
        {
            // TODO: implement topic handling
        }
        else if (cmd == "KICK") {
            _cmdKick(client, tokens);
        }
    }
}
