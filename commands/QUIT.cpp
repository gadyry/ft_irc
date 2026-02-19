# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"

void    Server::_handleQuit(Client* client)
{
    LOG(DISCONNECT, "Client fd = " << client->getFdClient() << " quit");
    removeClient(client->getFdClient());
}