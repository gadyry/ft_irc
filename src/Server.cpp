
#include "Server.cpp"

Server::Server() {}

Server::Server(u_short port, std::string password) : port(port), password(password), serv_fd(-1)
{
    sockaddr_in addr_in;

    this->serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_fd < 0)
        throw std::runtime_error("socket() failed");

    

    std::cout << "Server connected\n";
    std::cout << "Waiting for connection" << std::endl;
}

Server::~Server()
{
   close(serv_fd);
   // manage the resources!
}
