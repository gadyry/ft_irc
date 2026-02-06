# include "../includes/MovieBot.hpp"

MovieBot::MovieBot() : socketBot(-1), nick(""), user(""), servPort(0), hostname(""),
                    password(""), buffRecieve("") { }

MovieBot::MovieBot(std::string host, u_short port, std::string password) : socketBot(-1), nick("MovieBot"), user("MovieBot test test test"), servPort(port),
                    hostname(host), password(password), buffRecieve("") { }

MovieBot::~MovieBot()
{
    // manage the resources!!!
    close(this->socketBot);
}

// methods 

void     MovieBot::connectToServer()
{
    struct sockaddr_in  addr_serv;
    int                 camus = 1;

    this->socketBot = socket(PF_INET, SOCK_STREAM, 0);
    if (socketBot == -1)
        throw std::runtime_error("failed to create socket!");

    setsockopt(socketBot, SOL_SOCKET, SO_REUSEADDR, &camus, sizeof(camus));

    addr_serv.sin_family = PF_INET;
    addr_serv.sin_port = htons(this->servPort);
    
    if (inet_aton(this->hostname.c_str(), &addr_serv.sin_addr) <= 0)
        throw std::runtime_error("Invalid hostname");

    if (connect(this->socketBot, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)
		throw std::runtime_error("connect() failed");
}

void    MovieBot::executeMovieBot()
{
    this->connectToServer();
    // TODO: Implement bot logic
}