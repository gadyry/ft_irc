#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/IrcReplies.hpp"

void Server::_cmdKick(Client *client, std::vector<std::string> &tokens) {
    if (tokens.size() < 3) {
        std::string ermsg = ERR_NEEDMOREPARAMS(client->getNickname(), "KICK");
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return; 
    }
    std::string CHanL = tokens[1];
    Channel *channel = getChannel(CHanL);
    if (channel == NULL) {
        std::string ermsg = ERR_NOSUCHCHANNEL(client->getNickname(), CHanL);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return; 
    }
    if (!client->isInChannel(CHanL)) {
        std::string ermsg = ERR_NOTONCHANNEL(client->getNickname(), CHanL);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }
    if (!channel->isadmiin(client)) {
        std::string ermsg = ERR_CHANOPRIVSNEEDED(client->getNickname(), CHanL);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }
    std::string banned = tokens[2];
    Client *Toban = channel->getMemberName(banned);
    LOG(DEBUG, "|" << banned << "|");
    if (Toban == NULL) {
        std::string ermsg = ERR_USERNOTINCHANNEL(client->getNickname(), banned, CHanL);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }
    std::string reason = "Kicked OUT";
    if (tokens.size() > 3) {
        reason = tokens[3];
        for (size_t i = 4; i < tokens.size(); i++) {
            reason += " " + tokens[i];
        }
        if (reason[0] == ':')
            reason.erase(0, 1);
    }
    std::string kickedmsg = client->getNickname() + "!" + client->getUsername() + "@" + client->getHost();
    std::string kickBroad = RPL_KICK(kickedmsg, CHanL, banned, reason);
    channel->broadcast(kickBroad);
    channel->removeMember(Toban);
    LOG(INFO, client->getNickname() << " kicked " << banned << " from " << CHanL);
}