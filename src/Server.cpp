# include "../includes/Server.hpp"
# include "../includes/Client.hpp"

Server::Server()
{
    // handleCommand["NICK"] = &Server::_cmdNick;
    // handleCommand["USER"] = &Server::_cmdUser;
    // handleCommand["JOIN"] = &Server::_cmdJoin;
    // handleCommand["PRIVMSG"] = &Server::_cmdPrivmsg;
    // handleCommand["QUIT"] = &Server::_cmdQuit;
}

Server::Server(u_short port, std::string password) : port(port), password(password)
{
    struct sockaddr_in  addr_serv;
    int                 camus = 1;
    struct pollfd       newPollFd;

  this->serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_fd < 0)
        throw std::runtime_error("socket() failed");

    // std::memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = PF_INET;
    addr_serv.sin_port = htons(this->port);
    addr_serv.sin_addr.s_addr = INADDR_ANY;

	if(setsockopt(this->serv_fd, SOL_SOCKET, SO_REUSEADDR, &camus, sizeof(camus)) < 0)
		throw std::runtime_error("failed to set option (SO_REUSEADDR) on socket");

	if (fcntl(serv_fd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("fcntl() failed"));

    if (bind(serv_fd, (sockaddr*)&addr_serv, sizeof(addr_serv)) < 0)
        throw std::runtime_error("bind() failed");

    if (listen(serv_fd, 128) < 0)
        throw std::runtime_error("listen() failed");

    std::cout << "Server connected\n";
    std::cout << "Waiting for connection" << std::endl;

    newPollFd.fd = serv_fd;
    newPollFd.events = POLLIN;
    newPollFd.revents = 0;

    fds_sentinels.push_back(newPollFd);
}

void    Server::addClient()
{
    struct pollfd       newPoll;
    struct sockaddr_in  addr_client;
    socklen_t           address_len = sizeof(addr_client);

    int acpt = accept(this->serv_fd, (struct sockaddr*)&addr_client, &address_len);
    if (acpt == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        throw std::runtime_error("accept() failed");
    }

    // if (fcntl(acpt, F_SETFL, O_NONBLOCK) == -1)
    // {
    //     close(acpt);
    //     throw std::runtime_error("fcntl(O_NONBLOCK) failed");
    // }

    newPoll.fd = acpt;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    fds_sentinels.push_back(newPoll);

    clients[acpt] = new Client(acpt);

    std::cout << "New client: " << acpt << "connected" << std::endl;
}

void    Server::removeClient(int fd)
{
    delete clients[fd];
    clients.erase(fd);

    for(size_t i = 0; i <fds_sentinels.size(); i++)
    {
        if (fds_sentinels[i].fd == fd)
        {
            fds_sentinels.erase(fds_sentinels.begin() + i);
            break;
        }
    }
    close(fd);
}

void    Server::sendError(Client* client, const std::string& msg)
{
    /* this function will help you to handle all type errors possible without
    *  repeating msgs each time!!!!!
    */

    // for now:I need I send a simple msg, I should change it in future !!!
    send(client->getFdClient(), msg.c_str(), msg.length(), 0);
}


// 8=>  helpe me to split string to tokens d zeeb
std::vector<std::string> split_or(const std::string& str)
{
    std::vector<std::string> tokens;
    std::istringstream splitStr(str);
    std::string token;
    
    while (splitStr >> token)
        tokens.push_back(token);
    
    return (tokens);
}

void    Server::_regestrationIsValid(Client* client)
{
    if (client->checkAuthComplete())
    {
        std::string welcome = RPL_WELCOME(client->getNickname(), client->getUsername(), client->getHost());
        send(client->getFdClient(), welcome.c_str(), welcome.length(), 0);
    }
}

void    Server::_handleLine(Client* client, std::string& fullCmd)
{
    std::vector<std::string> tokens = split_or(fullCmd);
    std::string cmd = tokens[0];

    std::cout<<"leehwak rani dkhalt" << std::endl;
    if (!client->checkAuthComplete())
    {
        if (cmd == "PASS")
            this->_cmdPass(client, tokens);
        // else if (cmd == "NICK")
        //     this->_cmdNick(client, tokens);
        // else if (cmd == "USER")
        //     this->_cmdUser(client, tokens);
        // else if (cmd == "QUIT")
        //     this->_handleQuit(client);
        else
            this->sendError(client, ERR_NOTREGISTERED(client->getNickname()));

        this->_regestrationIsValid(client); // TODO

        return;
    }
    this->_handleCmd(client, tokens);
}

void    Server::processCmds(int fd)
{
    Client* client = clients[fd];
    if (!client) return;

    std::string& buffer = client->getInputBufferRef();
    size_t position = 0;

    while ((position = buffer.find("\r\n")) != std::string::npos)
    {
       std::string fullCmd = buffer.substr(0, position);
       buffer.erase(0, position + 2);

        if (fullCmd.empty()) continue;

        _handleLine(client, fullCmd);
    }
}

void    Server::recieveData(int fdClient)
{
    char buffer[BUFFER_SIZE] = {0};

    ssize_t bytes = recv(fdClient, buffer, sizeof(buffer) - 1, 0);

    if (bytes <= 0)
    {
        // TODO
        if (bytes == 0) // cleanup!!!!!!!!!!!!!!!
            std::cout << "Client disconnected: fd=" << fdClient << std::endl;
        else
            std::cerr << "recv() error on fd=" << fdClient << std::endl;
        removeClient(fdClient); // TODO !
        return;
    }

    clients[fdClient]->setInputBuffer(clients[fdClient]->getInputBuffer() + std::string(buffer, bytes));
    // comp cmd => (IRC commands end with \r\n)
        // =>TODO
    processCmds(fdClient);
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

    if (this->serv_fd != -1)
        close(this->serv_fd); // sakata zook dyal file descriptor
}
