
#include "Server.cpp"

Server::Server() {}

Server::Server(u_short port, std::string password) : port(port), password(password), serv_fd(-1)
{
    // init serv !!
}

Server::~Server()
{
   close(serv_fd);
   // manage the resources!
}

void    Server::createServShocket()
{
    this->serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serv_fd < 0)
    
}

