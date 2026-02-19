#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/IrcReplies.hpp"

void Server::_cmdInvite(Client *client, std::vector<std::string> &tokens) {
    if (tokens.size() < 3) {
        std::string ermsg = ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE");
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }

    std::string invitedUser = tokens[1];
    std::string chName = tokens[2];
    Channel *channel = getChannel(chName);
    if (!channel) {
        std::string ermsg = ERR_NOSUCHCHANNEL(client->getNickname(), chName);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }

    if (!channel->isMember(client)) {
        std::string ermsg = ERR_NOTONCHANNEL(client->getNickname(), chName);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }

    if (channel->isInviteOnly() && !channel->isadmiin(client)) {
        std::string ermsg = ERR_CHANOPRIVSNEEDED(client->getNickname(), chName);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }

    Client *TRclient = getClientName(invitedUser);
    if (!TRclient) {
        std::string ermsg = ERR_NOSUCHNICK(client->getNickname(), invitedUser);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }

    if (channel->isMember(TRclient)) {
        std::string ermsg = ERR_USERONCHANNEL(client->getNickname(), TRclient->getNickname(), chName);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }

    channel->addInvite(invitedUser);
    std::string inviteMsg = ":" + client->getNickname() + "!" + client->getUsername() + 
                            "@" + client->getHost() + " INVITE " + invitedUser + " :" + chName + "\r\n";
    send(TRclient->getFdClient(), inviteMsg.c_str(), inviteMsg.length(), 0);

    std::string notifyUser = RPL_INVITING(client->getNickname(), invitedUser, chName);
    send(client->getFdClient(), notifyUser.c_str(), notifyUser.length(), 0);
}
