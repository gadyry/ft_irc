#ifndef SERVER_HPP
#define SERVER_HPP

# include "Client.hpp"
# include "IrcReplies.hpp"
# include "Channel.hpp"
# include "IrcCommon.hpp"

class Client;
class Channel;
 
class Server
{
private :
	u_short       port;
	std::string password;
	int         serv_fd;
	std::string	servername;
	std::vector<struct pollfd> fds_sentinels;
	std::map<int, Client*> clients;
	std::map<std::string, Channel*> ch_channels;

	// typedef void (Server::*handleCommand)(Client*);
	// std::map<std::string, handleCommand> __commands; // the best & clean way to handle cmds , <<if, else == prostutation>>

	// methods: 
	void    addClient();
	void    recieveData(int fdClient);

	void    removeClient(int fd);
	void    sendError(Client* client, const std::string& msg);

	// utils handle cmd part:
	void    _regestrationIsValid(Client* client);
	void    _handleLine(Client* client, std::string& fullCmd);
	void    _handleCmd(Client* client, std::vector<std::string>& tokens);
	void    processCmds(int fd);

	// irc commands:
	// PASS
	void    _cmdPass(Client* client, std::vector<std::string>& tokens);
	// NICK
	bool    existNickname(std::string& nickname);
	void    _cmdNick(Client* client, std::vector<std::string>& tokens);
	// USER
	void    _cmdUser(Client* client, std::vector<std::string>& tokens);
	// JOIN
	void    _cmdJoin(Client* client, std::vector<std::string>& tokens);
	// TOPIC
	void    _cmdTopic(Client* client, std::vector<std::string>& tokens);
	// PRIVMSG
	void    _handlePrivmsg(Client* senderCl, std::vector<std::string>& tokens);
	//KICK
	void    _cmdKick(Client *client, std::vector<std::string> &tokens);
	//MODE
	void    _cmdMode(Client* client, std::vector<std::string>& tokens);
	//INVITE
	void _cmdInvite(Client *client, std::vector<std::string> &tokens);

	void    _handleQuit(Client* client);
	void	_cmdPingPong(Client* client, std::vector<std::string>& tokens);
	//channel
	Channel* getChannel(const std::string& name);
	Client* getClientName(std::string nick);

public :
	Server();
	Server(u_short port, std::string password);
	~Server();

	// methods :
	void    executeServ();
	static  void    signalHandler(int sig);
	static  int     g_signalReceived;
};

#endif