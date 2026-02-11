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
	void	dealWithPrivMsg(std::string& line);
	void	processMsg(std::string& fullMsg);
	void	handleMessage();
	void	buildBot();
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