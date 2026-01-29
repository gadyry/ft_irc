# include "Server.cpp"
# include "Client.cpp"

Server::void    _handleCmd(Client* client, std::vector<std::string>& tokens)
{
    std::string& cmd = tokens[0];

    if (client->getReg_stat())
    {
        if (cmd == "JOIN")
            // _handleJoin(...)
        else if (cmd == "TOPIC")
            // _handleTopic(...)
        // ...
    }
    // POUR SUIVRE !!!!!!!!!!!
}
