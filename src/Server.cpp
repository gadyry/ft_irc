# include "../includes/Server.hpp"
# include "../includes/Client.hpp"

void    log(LogLevel level, const std::string &msg)
{
    switch (level)
    {
        case INFO:
            std::cout << C_GREEN << C_BOLD << "[SERVER] " << C_RESET << C_GREEN << msg << C_RESET << std::endl;
            break;
        case WARN:
            std::cout << C_YELLOW << "[WARN] " << C_RESET << C_YELLOW << msg << C_RESET << std::endl;
            break;
        case ERROR:
            std::cerr << C_RED << C_BOLD << "[ERROR] " << C_RESET << C_RED << msg << C_RESET << std::endl;
            break;
        case DEBUG:
            std::cout << C_BLUE << "[DEBUG] " << C_RESET << C_BLUE << msg << C_RESET << std::endl;
            break;
        case CLIENT:
            std::cout << C_MAGENTA << "[CLIENT] " << C_RESET << C_MAGENTA << msg << C_RESET << std::endl;
            break;
        case NEWCLIENT:
            std::cout << C_CYAN << "[NEW CLIENT] " << C_RESET << C_CYAN << msg << C_RESET << std::endl;
            break;
        case DISCONNECT:
            std::cout << C_MAGENTA << "[DISCONNECT] " << C_RESET << C_MAGENTA << msg << C_RESET << std::endl;
            break;
    }
}

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

    LOG(INFO, "Server connected");
    LOG(INFO, "Waiting for connection");

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

    LOG(NEWCLIENT, "New client: fd = " << acpt << " connected");
}

void    Server::removeClient(int fd)
{
    std::map<int, Client*>::iterator it = clients.find(fd);
    if (it != clients.end())
    {
        delete it->second;
        clients.erase(it);
    }

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
    // for now:I need I send a simple msg, I should change it in future !!!
    send(client->getFdClient(), msg.c_str(), msg.length(), 0);
}

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
        std::string welcome = RPL_WELCOME(client->getNickname());
        send(client->getFdClient(), welcome.c_str(), welcome.length(), 0);
    }
}

void    Server::_handleLine(Client* client, std::string& fullCmd)
{
    std::vector<std::string> tokens = split_or(fullCmd);
    if (tokens.empty()) return;
    
    std::string cmd = tokens[0];

    if (cmd == "QUIT")
    {
        this->_handleQuit(client); return;
    }

    if (!client->checkAuthComplete())
    {
        if (cmd == "PASS")
            this->_cmdPass(client, tokens);
        else if (cmd == "NICK" && client->getAuthState() >= AUTH_PASS_OK)
            this->_cmdNick(client, tokens);
        else if (cmd == "USER" && client->getAuthState() >= AUTH_PASS_OK)
            this->_cmdUser(client, tokens);
        else
        {
            this->sendError(client, ERR_NOTREGISTERED()); return;
        }

        this->_regestrationIsValid(client);
        return;
    }
    this->_handleCmd(client, fullCmd,tokens);
}

void    Server::processCmds(int fd)
{
    Client* client = clients[fd];
    if (!client) return;

    std::string& buffer = client->getInputBufferRef();
    size_t position;

    while (69)
    {
        position = buffer.find("\r\n");
        size_t len_spec = 2;
        if (position == std::string::npos)
        {
            position = buffer.find("\n");
            len_spec = 1;
        }

        if (position == std::string::npos) break;

        std::string fullCmd = buffer.substr(0, position);
        buffer.erase(0, position + len_spec);

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
        if (bytes == 0)
            LOG(DISCONNECT, "Client disconnected: fd = " << fdClient);
        else
            LOG(ERROR, "recv() error on fd = " << fdClient);
        removeClient(fdClient);
        return;
    }

    clients[fdClient]->setInputBuffer(clients[fdClient]->getInputBuffer() + std::string(buffer, bytes));
    processCmds(fdClient);
}

void    Server::executeServ()
{
    // TODO 8=> I should handle the signal after building the serv
    while (69)
    {
        if (poll(&fds_sentinels[0], fds_sentinels.size(), 0) == -1 )
            throw std::runtime_error("poll() failed");

        for (size_t i = 0; i < fds_sentinels.size(); i++)
        {
            if (fds_sentinels[i].revents && POLLIN)
            {
                if (fds_sentinels[i].fd == serv_fd)
                    addClient();
                else
                    recieveData(fds_sentinels[i].fd);
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


//channel
Channel* Server::getChannel(const std::string& name) {
    std::map<std::string, Channel*>::iterator it = ch_channels.find(name);
    if (it != ch_channels.end()) {
        return it->second;
    }
    return NULL;
}