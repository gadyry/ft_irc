
#include "../includes/Server.hpp"

Server::Server() {}

Server::Server(short port, std::string password) : port(port), password(password), serv_fd(-1)
{
    struct sockaddr_in addr_serv;
    int camus = 1;

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

	if(setsockopt(this->serv_fd, SOL_SOCKET, SO_REUSEADDR, &camus, sizeof(camus)) < 0)
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));

    if (bind(serv_fd, (sockaddr*)&addr_serv, sizeof(addr_serv)) < 0)
        throw std::runtime_error("bind() failed");

    if (listen(serv_fd, 128) < 0) // int listen(int socket, int backlog); try to set it to 0
        throw std::runtime_error("listen() failed");

    std::cout << "Server connected\n";
    std::cout << "Waiting for connection" << std::endl;

    // accepting connection request
    int clientSocket = accept(serv_fd, NULL, NULL);
    // recieving data
    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer
              << std::endl;
}

void    Server::executeServ()
{
    // TODO
    int turn_up = 1;
    while (turn_up)
    {
        // check if the serv_fd == to fd_pull
        /*     accpet     */
    }
}

Server::~Server()
{
   close(this->serv_fd);
   // manage the resources!
}
