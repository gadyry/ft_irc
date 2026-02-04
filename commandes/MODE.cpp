#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/IrcReplies.hpp"

void   Server::_cmdMode(Client* client, std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::string ermsg = ERR_NEEDMOREPARAMS(client->getNickname(), "MODE");
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return; 
    }
    std::string target = tokens[1];
    if (target[0] != '#')
        return ;
    Channel *channel = getChannel(target);
    if (!channel) {
        std::string ermsg = ERR_NOSUCHCHANNEL(client->getNickname(), target);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }
    if (tokens.size() == 2) {
        std::string modeStr = channel->getChFlags();
        std::string msg = ":IRCServer 324 " + client->getNickname() + " " + target + " " + modeStr + "\r\n";
        send(client->getFdClient(), msg.c_str(), msg.length(), 0);
        return;
    }
    if (!channel->isadmiin(client)) {
        std::string ermsg = ERR_CHANOPRIVSNEEDED(client->getNickname(), target);
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }
    std::string modeFlags = tokens[2];
    bool adding = true;
    std::string execModes = "";
    for (size_t i = 0; i < modeFlags.size(); i++) {
        char mode = modeFlags[i];
        if (mode == '+') {
            adding = true;
            execModes += "+";
            continue;
        } 
        if (mode == '-') {
            adding = false;
            execModes += "-";
            continue;
        }
        if (mode == 'i') {
            channel->setInviteOnly(adding);
            execModes += "i";
        }
        // future flags heeere 
        /*
        */
        else {
            
        }
    }
    if (!execModes.empty() && execModes != "+" && execModes != "-") {
        std::string broadcastMsg = ":" + client->getNickname() + "!" + client->getUsername() +
            "@" + client->getHost() + " MODE " + target + " :" + execModes + "\r\n";
        channel->broadcast(broadcastMsg);
    }
}