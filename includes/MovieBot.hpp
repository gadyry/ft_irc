#ifndef MOVIEBOT_HPP
#define MOVIEBOT_HPP

# include "Client.hpp"
# include "IrcReplies.hpp"
# include "IrcCommon.hpp"

class MovieBot
{
private:
	// Attribut :
	int				socketBot;
	std::string     hostname;
	std::string     nick;
	std::string     user;
	unsigned short  servPort;
	std::string     password;
	std::string     recieveBuff;

	// methods :
	void	connectToServer();
	void	sendPrivMsg(std::string& target, std::string& message);
	void	dealWithPrivMsg(std::string& prefix, std::vector<std::string>& args);
	void	processMsg(std::string& fullMsg);
	void	handleMessage();
	void	buildBot();
	// 8=> cmds methods
	std::string handleHelp();
	std::string handleQuote(std::vector<std::string>& args);
	std::string handleList(std::vector<std::string>& args);
	std::string handleAdd(std::vector<std::string>& args, std::string& sender);
	std::string handleInfo(std::vector<std::string>& args);
public:
	// Constractor && Distractor:
		MovieBot();
		MovieBot(std::string host, u_short port, std::string password);
		~MovieBot();
	
	// Setters && Getters

	// methods helper
	void    executeMovieBot();
};

#endif