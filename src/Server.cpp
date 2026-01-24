
# include "../includes/Server.hpp"
# include "../includes/Client.hpp"

Server::Server() {}

Server::Server(short port, std::string password) : port(port), password(password), serv_fd(-1)
{
    struct sockaddr_in  addr_serv;
    int                 camus = 1;
    struct pollfd       newPollFd;

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
		throw std::runtime_error("failed to set option (SO_REUSEADDR) on socket");

	if (fcntl(serv_fd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("fcntl() failed"));

    if (bind(serv_fd, (sockaddr*)&addr_serv, sizeof(addr_serv)) < 0)
        throw std::runtime_error("bind() failed");

    if (listen(serv_fd, 128) < 0) // int listen(int socket, int backlog); try to set it to 0
        throw std::runtime_error("listen() failed");

    std::cout << "Server connected\n";
    std::cout << "Waiting for connection" << std::endl;

    newPollFd.fd = serv_fd; // WHO to watch: which socket!
    newPollFd.events = POLLIN; // WHAT I want to watch: who care about !
    newPollFd.revents = 0; // WHAT actually happened (kernel writes this) disappears, I can leave it without assign it!

    fds_sentinels.push_back(newPollFd);

    /********* This next step just for testing: ********/

    // accepting connection request
    int clientSocket = accept(serv_fd, NULL, NULL);
    // recieving data
    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer
              << std::endl;
}

void    Server::addClient()
{
    struct pollfd       newPoll;
    Client              client;
    struct sockaddr_in  addr_client;
    socklen_t           address_len = sizeof(addr_client);

    int acpt = accept(this->serv_fd, (struct sockaddr*)&addr_client, &address_len);
    if (acpt == -1)
        throw std::runtime_error("accept() failed");

    newPoll.fd = acpt;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    fds_sentinels.push_back(newPoll);
    clients[acpt] = "";

    std::cout << "New client: " << acpt << "connected" << std::endl;
}

void    Server::recieveData(int fdClient)
{
    char buffer[1024] = {0};

    ssize_t bytes = recv(fdClient, buffer, sizeof(buffer), 0);
    /*
            recv()
        │
        ├── bytes > 0
        │     └── append data to Client buffer
        │
        ├── bytes == 0
        │     └── client disconnected → cleanup
        │
        └── bytes < 0
            ├── errno == EAGAIN / EWOULDBLOCK
            │     └── ignore
            │
            └── else
                    └── error → cleanup

    */
    if (bytes > 0)
    {
        // TODO
    }
    else
    {
        // TODO
    }
}

void    Server::executeServ()
{
    // TODO 8=> I should handle the signal after building the serv
    while (69)
    {
        /*
            timeout = -1   // wait forever
            timeout = 0    // do not wait (non-blocking check)
            timeout > 0    // wait N milliseconds
        */
        if (poll(&fds_sentinels[0], fds_sentinels.size(), 0) == -1 )
            throw std::runtime_error("poll() failed");

        for (size_t i = 0; i < fds_sentinels.size(); i++)
        {
            if (fds_sentinels[i].revents && POLLIN)
            {
                /*    -> add a new client     */
                if (fds_sentinels[i].fd == serv_fd)
                    addClient();
                /*    -> recieve a new Data   */
                else
                    recieveData(fds_sentinels[i].fd); // TODO
            }
        }
    }
}

Server::~Server()
{
   // manage the resources!
    // for (size_t i = 1; i < clients.size(); i++)
    // {
    //     std::cout << "Client: " << this->clients[i].getFdClient() << "disconnected!" << std::endl;
    //     close(this->clients[i].getFdClient());
    // }

    if (this->serv_fd != -1)
        close(this->serv_fd); // sakata zook dyal file descriptor
}
