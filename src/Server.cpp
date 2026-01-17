
#include "../includes/Server.hpp"

Server::Server() {}

Server::Server(short port, std::string password) : port(port), password(password), serv_fd(-1)
{
    sockaddr_in addr_serv;

    this->serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_fd < 0)
        throw std::runtime_error("socket() failed");

    /*
        struct sockaddr_in
        {
            sa_family_t    sin_family;   // Address family (AF_INET)
            in_port_t      sin_port;     // Port number (network byte order)
            struct in_addr sin_addr;     // IPv4 address
            unsigned char  sin_zero[8];  // Padding (unused)
        };
    */
    addr_serv.sin_family = PF_INET;
    addr_serv.sin_port = htons(this->port); // Host TO Network Short
    addr_serv.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces (0.0.0.0)

    if (bind(serv_fd, (sockaddr_in*)&addr_serv, sizeof(addr_serv)) < 0)
        throw std::runtime_error("bind() failed");

    if (listen(serv_fd, 128) < 0) // int listen(int socket, int backlog); try to set it to 0
        throw std::runtime_error("listen() failed");

    std::cout << "Server connected\n";
    std::cout << "Waiting for connection" << std::endl;
}

Server::~Server()
{
   close(serv_fd);
   // manage the resources!
}
