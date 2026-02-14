#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/IrcReplies.hpp"

void Server::_cmdTopic(Client *client, std::vector<std::string> &tokens) {
    if (tokens.size() < 2) {
        std::string ermsg = ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC");
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }
    std::string chName = tokens[1];
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
    
    if (tokens.size() == 2) {
        if (channel->ch_getTopic().empty()) {
            std::string msg = RPL_NOTOPIC(client->getNickname(), chName);
            send(client->getFdClient(), msg.c_str(), msg.length(), 0);
        }
        else {
            std::string msg = RPL_TOPIC(client->getNickname(), chName, channel->ch_getTopic());
            send(client->getFdClient(), msg.c_str(), msg.length(), 0);
        }
        return;
    }

    if (channel->isTopicAdmOnly() && !channel->isadmiin(client)) {
        std::string ermsg = ERR_CHANOPRIVSNEEDED(client->getNickname(), chName);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }

    std::string seTopic = "";
    for (size_t i = 2; i < tokens.size(); i++) {
        seTopic += tokens[i];
        if (i < tokens.size() - 1)
            seTopic += " ";
    }
    if (seTopic.length() > 0 && seTopic[0] == ':') {
        seTopic = seTopic.substr(1);
    }
    channel->setTopic(seTopic);
    std::string topicMsg = ":" + client->getNickname() + " TOPIC " + chName + " :" + seTopic + "\r\n";
    channel->broadcast(topicMsg);
}