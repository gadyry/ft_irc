# include "../includes/Client.hpp"


// Constructor
Client::Client()
    : fd_client(-1), nickname(""), username(""), host(""), inputBuffer(""), reg_stat(false) {}

// Destructor
Client::~Client() {}

// Setters
void Client::setFdClient(short fd)
{
    fd_client = fd;
}
void Client::setNickname(const std::string &nick)
{
    nickname = nick;
}
void Client::setUsername(const std::string &user)
{
    username = user;
}

void Client::setHost(const std::string &h)
{
    this->host = h;
}
void Client::setInputBuffer(const std::string &buf)
{
    inputBuffer = buf;
}

void Client::setRegStat(bool status)
{
    reg_stat = status;
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
    return inputBuffer;
}
bool Client::getReg_stat()
{
    return (reg_stat);
}

// Utility
void Client::clearInputBuffer()
{
    inputBuffer.clear();
}
