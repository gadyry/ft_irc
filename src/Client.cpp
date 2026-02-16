# include "../includes/Client.hpp"

// Constructor
Client::Client()
	: fd_client(-1), nickname(""), username(""), host(""), inputBuffer(""), authState(AUTH_DISCONNECTED) {}

Client::Client(int fd)
	: fd_client(fd), nickname(""), username(""), host(""), inputBuffer(""), authState(AUTH_DISCONNECTED) {}

// Destructor
Client::~Client() {}

// Setters
void Client::setFdClient(short fd)
{
	this->fd_client = fd;
}
void Client::setNickname(const std::string &nick)
{
	this->nickname = nick;
}
void Client::setUsername(const std::string &user)
{
	this->username = user;
}

void Client::setHost(const std::string &h)
{
	this->host = h;
}
void Client::setInputBuffer(const std::string &buf)
{
	this->inputBuffer = buf;
}

// Getters
short Client::getFdClient()
{ 
	return (fd_client);
}
std::string Client::getNickname()
{
	return (nickname);
}
std::string Client::getUsername()
{ 
	return (username);
}
std::string Client::getHost()
{
	return (host);
}
std::string Client::getInputBuffer()
{ 
	return (inputBuffer);
}
std::string& Client::getInputBufferRef()
{ 
	return (inputBuffer);
}

// Authentication methods
void    Client::setPassOk()
{
	if (authState == AUTH_DISCONNECTED)
		authState = AUTH_PASS_OK;
}
void    Client::setNickOk()
{
	if (authState >= AUTH_PASS_OK)
	{
		if (authState == AUTH_PASS_CMD_OK)
			authState = AUTH_COMPLETE;
		else
			authState = AUTH_PASS_CMD_OK;
	}
}
void    Client::setUserOk()
{
	if (authState >= AUTH_PASS_OK)
	{
		if (authState == AUTH_PASS_CMD_OK)
			authState = AUTH_COMPLETE;
		else
			authState = AUTH_PASS_CMD_OK;
	}
}
AuthState   Client::getAuthState()
{
	return (authState);
}
bool    Client::checkAuthComplete()
{
	return (authState == AUTH_COMPLETE);
}

void Client::addChannel(const std::string &channelName) {
	joinedChannels.insert(channelName);
}

void Client::removeChannel(const std::string &channelName) {
	joinedChannels.erase(channelName);
}

bool Client::isInChannel(const std::string &channelName) {
	return joinedChannels.find(channelName) != joinedChannels.end();
}