#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/IrcReplies.hpp"

void Server::_cmdJoin(Client *client, std::vector<std::string> &tokens)
{
    if (tokens.size() < 2) {
        std::string ermsg = ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN");
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return; 
    }

    std::string chName = tokens[1];
    if (chName[0] != '#' || chName.size() < 2) {
        std::string ermsg = ERR_NOSUCHCHANNEL(client->getNickname(), chName);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }

    Channel *channel = getChannel(chName);
    if (channel == NULL) {
        channel = new Channel(chName);
        ch_channels[chName] = channel;
    }

    if (channel->isInviteOnly() && !channel->isInvited(client->getNickname())) {
        std::string ermsg = ERR_INVITEONLYCHAN(client->getNickname(), chName);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }

    channel->addMember(client);
    /*theyre added so removing the invite in xase of leaving they wont have access to join agn they need a new
    invite token*/
    channel->removeInvite(client->getNickname());

    std::string jMsg = client->getNickname() + "!" + client->getUsername() + "@" + client->getHost();
    std::string joinMsg = RPL_JOIN(jMsg, chName);
    channel->broadcast(joinMsg);

    if (channel->ch_getTopic().empty()) {
        std::string noTopMsg = RPL_NOTOPIC(client->getNickname(), chName);
        send(client->getFdClient(), noTopMsg.c_str(), noTopMsg.length(), 0);
    }
    else {
        std::string topicMsg = RPL_TOPIC(client->getNickname(), chName, channel->ch_getTopic());
        send(client->getFdClient(), topicMsg.c_str(), topicMsg.length(), 0);
    }

    std::string Ulist = channel->getUserList();
    std::string nsMsg = RPL_NAMREPLY(client->getNickname(), "=", chName, Ulist);
    send(client->getFdClient(), nsMsg.c_str(), nsMsg.length(), 0);

    std::string endofNames = RPL_ENDOFNAMES(client->getNickname(), chName);
    send(client->getFdClient(), endofNames.c_str(), endofNames.length(), 0);
}