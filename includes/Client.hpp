#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <set>

# define LIMIT_NICK 9

enum AuthState
{
	AUTH_DISCONNECTED,
	AUTH_PASS_OK,
	AUTH_PASS_CMD_OK,
	AUTH_COMPLETE
};

class Client
{
private:
	int                     fd_client;
	std::string             nickname;
	std::string             username;
	std::string             host;
	std::string             inputBuffer;
	AuthState               authState;
	std::set<std::string>   joinedChannels; // chose of set : ignore the duplicate strings!

public:
	// Constructor & Destructor
	Client();
	Client(int fd);
	~Client();

	// Setters
	void setFdClient(short fd);
	void setNickname(const std::string &nick);
	void setUsername(const std::string &user);
	void setHost(const std::string &h);
	void setInputBuffer(const std::string &buf);

	// Getters
	short           getFdClient();
	std::string     getNickname();
	std::string     getUsername();
	std::string     getHost();
	std::string     getInputBuffer();
	std::string&    getInputBufferRef();

	// Authentication methods
	void        setPassOk();
	void        setNickOk();
	void        setUserOk();
	AuthState   getAuthState();
	bool        checkAuthComplete();

	//channel
	void addChannel(const std::string &channelName);
	void removeChannel(const std::string &channelName);
	bool isInChannel(const std::string &channelName);
};

#endif