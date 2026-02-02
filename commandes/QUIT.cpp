# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include "../includes/IrcReplies.hpp"

void    Server::_handleQuit(Client* client)
{
    // RFC 1459: QUIT [:<Quit Message>]
    // The QUIT command is used to either close the connection by the client
    // when its done or to ask the server to close the connection.
    
    // For now: Simply disconnect the client
    // TODO: Notify other clients in shared channels before disconnecting
    
    LOG(DISCONNECT, "Client fd=" << client->getFdClient() << " quit");
    removeClient(client->getFdClient());
}