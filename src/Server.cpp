
#include "Server.cpp"

Server::Server() {}

Server::Server(u_short port, std::string password) : port(port), password(password), serv_fd(-1)
{
    this->serv_fd = socket(IPV4, TCP_SOCK, 0);
    if (serv_fd < 0)
        throw std::runtime_error("socket() failed");

    
}

Server::~Server()
{
   close(serv_fd);
   // manage the resources!
}

