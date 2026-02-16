# include "../includes/Server.hpp"
# include "../includes/Client.hpp"
# include <string>

void	Server::_cmdPingPong(Client* client, std::vector<std::string>& tokens)
{
	if (tokens.size() < 2)
	{
		this->sendError(client, ERR_NOORIGIN(client->getNickname()));
		return;
	}

	std::string token = tokens[1];
	if (!token.empty() && token[0] == ':')
		token = token.substr(1);

	if (tokens.size() >= 3)
	{
		std::string targetServer = tokens[2];

		if (targetServer != this->servername)
		{
			this->sendError(client, ERR_NOSUCHSERVER(client->getNickname(), targetServer));
			return;
		}
	}

	if (tokens[0] == "PING")
	{
		std::string pongMsg = RPL_PONG(this->servername, token);

		if (send(client->getFdClient(), pongMsg.c_str(), pongMsg.length(), 0) < 0)
			LOG(ERROR, "send() failed for PONG");
		else
			LOG(INFO, "Responded to PING with token '" << token << "'");

		return;
	}
	else if (tokens[0] == "PONG")
	{
		client->markAlive();
		client->updateLastPongTime();

		LOG(INFO, "Received PONG from client with token '" << token << "'");
		return;
	}
}


void Server::_handleCmd(Client* client, std::vector<std::string>& tokens)
{
	std::string cmd = tokens[0];

	if (client->checkAuthComplete())
	{
		if (cmd == "JOIN")
		{
			_cmdJoin(client, tokens); return;
		}
		else if (cmd == "KICK")
		{
			_cmdKick(client, tokens); return;
		}
		else if (cmd == "PRIVMSG")
		{   
			_handlePrivmsg(client, tokens); return;
		}
		else if (cmd == "MODE")
		{
			_cmdMode(client, tokens); return;
		}
		else if (cmd == "INVITE") {
			_cmdInvite(client, tokens); return;
		}
		else if (cmd == "TOPIC") {
			_cmdTopic(client, tokens); return;
		}
		else
		{
			this->sendError(client, ERR_NOTREGISTERED()); return;
		}
	}
}
