#ifndef IRC_COMMON_HPP
#define IRC_COMMON_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <algorithm>
# include <sys/socket.h>
# include <sys/types.h>
# include <fcntl.h>
# include <poll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <errno.h>
# include <istream>
# include <signal.h>
# include <ctime>

# define BUFFER_SIZE 512

# define C_RESET   "\033[0m"
# define C_BOLD    "\033[1m"
# define C_RED     "\033[31m"
# define C_GREEN   "\033[32m"
# define C_YELLOW  "\033[33m"
# define C_BLUE    "\033[34m"
# define C_MAGENTA "\033[35m"
# define C_CYAN    "\033[36m"

// ============= IRC codes (for IRC messages)=============
// # define IRC_BOLD   "\x02"
// # define IRC_RESET  "\x0F"
// # define IRC_GREEN  "\x0303"
// # define IRC_CYAN   "\x0311"

// # define IRC_CMD(text) std::string(IRC_GREEN) + text + IRC_RESET

enum LogLevel
{
	INFO,
	WARN,
	ERROR,
	DEBUG,
	CLIENT,
	NEWCLIENT,
	DISCONNECT,
	BOT
};

void	log(LogLevel level, const std::string &msg);

# define LOG(level, msg) \
	do { std::ostringstream ss; ss << msg; log(level, ss.str()); } while (0)

#endif
