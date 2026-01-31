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
            // TODO: implement join handling
        }
        else if (cmd == "TOPIC")
        {
            // TODO: implement topic handling
        }
        else if (cmd == "JOIN")
        {
            // TODO: implement JOIN handling
        }
        // ...
    }
}
