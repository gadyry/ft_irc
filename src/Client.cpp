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
    if (authState == AUTH_PASS_OK)
        authState = AUTH_PASS_NICK_OK;
}
void    Client::setUserOk()
{
    if (authState == AUTH_PASS_NICK_OK)
        authState = AUTH_COMPLETE;
}

bool    Client::checkAuthComplete()
{
    return (authState == AUTH_COMPLETE);
}

// Utility
void Client::clearInputBuffer()
{
    inputBuffer.clear();
}
