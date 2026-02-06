# include "../includes/MovieBot.hpp"

MovieBot::MovieBot() : socketBot(-1), nick(""), user(""), servPort(0), hostname("")
                    password(password), buffRecieve("") { }

MovieBot::MovieBot(std:string host, u_short port, std::string password) : socketBot(-1), nick(""), user(""), servPort(port),
                    hostname(host), password(password), buffRecieve("") { }

MovieBot::~MovieBot()
{
    // manage the resources!!!
    close(this->socketBot);
}

// methods 

void     MovieBot::connect()
{
    struct sockaddr_in  addr_serv;
    int                 camus = 1;
    struct pollfd       newPollFd;

    this->socketBot = socket(PF_INET, SOCK_STREAM, 0);
    if (socketBot == -1)
        throw std::runtime_error("failed to create socket!");

    addr_serv.sin_family = PF_INET;
    addr_serv.sin_port = htons(this->servPort);

    if (connect(socketBot, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		throw std::runtime_error("connect() failed");
}

// void    MovieBot::executeMovieBot()
// {
//     this->connect(); // TODO

// }