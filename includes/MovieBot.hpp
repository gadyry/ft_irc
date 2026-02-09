#ifndef MOVIEBOT_HPP
#define MOVIEBOT_HPP

# include <iostream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <sys/types.h>
# include <fcntl.h>        // fcntl
# include <poll.h>         // struct pollfd, poll()
# include <netinet/in.h>   // sockaddr_in, INADDR_ANY
# include <arpa/inet.h>    // htons, htonl, inet_aton
# include <unistd.h>       // close
# include <errno.h>
# include <sstream>        // for istingstream
# include <istream>        // for getline

# include "Client.hpp"
# include "IrcReplies.hpp"

# define C_RESET   "\033[0m"
# define C_BOLD    "\033[1m"
# define C_RED     "\033[31m"
# define C_GREEN   "\033[32m"
# define C_YELLOW  "\033[33m"
# define C_BLUE    "\033[34m"
# define C_MAGENTA "\033[35m"
# define C_CYAN    "\033[36m"

class MovieBot
{
private:
	// Attribut :
	int             socketBot;
	std::string     hostname;
	std::string     nick;
	std::string     user;
	unsigned short  servPort;
	std::string     password;
	std::string     buffRecieve;

	// methods :
	void    connectToServer();
	void	handleMessage(std::string& msg);
	void    buildBot();
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