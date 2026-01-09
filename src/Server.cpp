
#include "Server.cpp"

Server::Server() {}

Server::Server(u_short port, std::string password)
{
    this->port = port;
    this->password = password;
    this->serv_fd = -1;

}

Server::~Server() {}

void    Server::createServShocket()
{
    this->serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serv_fd < 0)
        throw std::runtime_error("socket() failed");
}

void    Server::initializeserv()
{
    createServShocket();
    // What I need after this ?
}