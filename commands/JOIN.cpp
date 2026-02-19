#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/IrcReplies.hpp"
#include "../includes/Server.hpp"

std::vector<std::string> splitByComma(const std::string &str) {
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, ',')) {
		if (!item.empty())
			result.push_back(item);
	}
	return result;
}

void Server::_cmdJoin(Client *client, std::vector<std::string> &tokens) {
	if (tokens.size() < 2) {
		std::string ermsg = ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN");
		send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
		return;
	}

	std::vector<std::string> channels = splitByComma(tokens[1]);
	std::vector<std::string> keys;
	if (tokens.size() > 2)
		keys = splitByComma(tokens[2]);

	for (size_t i = 0; i < channels.size(); ++i) {
		std::string chName = channels[i];
		std::string key = (i < keys.size()) ? keys[i] : "";

		if (chName[0] != '#' || chName.size() < 2) {
			std::string ermsg = ERR_NOSUCHCHANNEL(client->getNickname(), chName);
			send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
			continue;
		}

		Channel *channel = getChannel(chName);
		if (channel == NULL) {
			channel = new Channel(chName);
			ch_channels[chName] = channel;
		}

		if (channel->isMember(client)) {
			std::string ermsg = ERR_USERONCHANNEL(client->getNickname(), client->getNickname(), chName);
			send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
			continue;
		}

		if (channel->isInviteOnly() && !channel->isInvited(client->getNickname())) {
			std::string ermsg = ERR_INVITEONLYCHAN(client->getNickname(), chName);
			send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
			continue;
		}

		if (!channel->getPassKey().empty()) {
			if (key != channel->getPassKey()) {
				std::string ermsg = ERR_BADCHANNELKEY(client->getNickname(), chName);
				send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
				continue;
			}
		}

		if (channel->getLimitUser() > 0 && channel->getMemSize() >= channel->getLimitUser()) {
			std::string ermsg = ERR_CHANNELISFULL(client->getNickname(), chName);
			send(client->getFdClient(), ermsg.c_str(), ermsg.length(), 0);
			continue;
		}
		channel->addMember(client);
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
}