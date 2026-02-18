#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/IrcReplies.hpp"

void Server::_cmdMode(Client* client, std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::string ermsg = ERR_NEEDMOREPARAMS(client->getNickname(), "MODE");
        send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
        return;
    }
    std::string target = tokens[1];
    if (target[0] != '#')
        return;

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
    size_t argIndex = 3;
    char currentSign = '+';
    char lastSign = 0;
    std::string modeString;
    std::vector<std::string> modeArgs;

    for (size_t i = 0; i < modeFlags.size(); ++i) {
        char c = modeFlags[i];
        if (c == '+' || c == '-') {
            currentSign = c;
            continue;
        }

        if (c == 'i' || c == 't' || c == 'k' || c == 'l' || c == 'o') {
            bool requiresArg = false;
            if (c == 'k' && currentSign == '+')
                requiresArg = true;
            if (c == 'l' && currentSign == '+')
                requiresArg = true;
            if (c == 'o')
                requiresArg = true;
            std::string arg;
            if (requiresArg) {
                if (argIndex >= tokens.size()) {
                    std::string err = ERR_NEEDMOREPARAMS(client->getNickname(), "MODE");
                    send(client->getFdClient(), err.c_str(), err.length(), 0);
                    continue;
                }
                arg = tokens[argIndex++];
            }
            if (c == 'i') {
                channel->setInviteOnly(currentSign == '+');
            }
            else if (c == 't') {
                channel->setTopicAdmOnly(currentSign == '+');
            }
            else if (c == 'k') {
                if (currentSign == '+')
                    channel->setPassKey(arg);
                else
                    channel->setPassKey("");
            }
            else if (c == 'l') {
                if (currentSign == '+') {
                    int limit = std::atoi(arg.c_str());
                    if (limit > 0)
                        channel->setLimitUser(static_cast<size_t>(limit));
                }
                else {
                    channel->setLimitUser(0);
                }
            }
            else if (c == 'o') {
                Client *targetClient = channel->getMemberName(arg);
                if (targetClient) {
                    if (currentSign == '+')
                        channel->addadmiin(targetClient);
                    else
                        channel->removeadmiin(targetClient);
                }
                else {
                    std::string ermsg = ERR_USERNOTINCHANNEL(client->getNickname(), arg, target);
                    send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
                    continue;
                }
            }
            if (lastSign != currentSign) {
                modeString += currentSign;
                lastSign = currentSign;
            }
            modeString += c;
            if (requiresArg && !arg.empty())
                modeArgs.push_back(arg);

        }
        else {
            std::string err = ERR_UNKNOWNMODE(client->getNickname(), std::string(1, c));
            send(client->getFdClient(), err.c_str(), err.length(), 0);
        }
    }
    if (!modeString.empty()) {
        std::string broadcastMsg = ":" + client->getNickname() + "!" + client->getUsername() +
                                   "@" + client->getHost() + " MODE " + target + " " + modeString;
        for (size_t i = 0; i < modeArgs.size(); ++i)
            broadcastMsg += " " + modeArgs[i];
        broadcastMsg += "\r\n";
        channel->broadcast(broadcastMsg);
    }
}