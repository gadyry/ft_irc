# include "../includes/MovieBot.hpp"
# include "../includes/Server.hpp"

MovieBot::MovieBot() : socketBot(-1), hostname(""), nick(""), user(""), servPort(6969),
					password(""), buffRecieve("") { }

MovieBot::MovieBot(std::string host, u_short port, std::string password) : socketBot(-1), hostname(host), nick("MovieBot"), user("MovieBot test test test"),
					servPort(port), password(password), buffRecieve("") { }

MovieBot::~MovieBot()
{
	// manage the resources!!!
	close(this->socketBot);
}

// methods 
void	MovieBot::connectToServer()
{
	struct sockaddr_in	addr_serv;
	int					camus = 1;

	this->socketBot = socket(PF_INET, SOCK_STREAM, 0);
	if (socketBot == -1)
		throw std::runtime_error("failed to create socket!");

	setsockopt(socketBot, SOL_SOCKET, SO_REUSEADDR, &camus, sizeof(camus));

	addr_serv.sin_family = PF_INET;
	addr_serv.sin_port = htons(this->servPort);
	
	if (inet_aton(this->hostname.c_str(), &addr_serv.sin_addr) <= 0)
		throw std::runtime_error("Invalid hostname!");

	if (connect(this->socketBot, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)
		throw std::runtime_error("connect() failed!");

	std::vector<std::string> AuthCmds;
	AuthCmds.push_back("PASS " + password + "\r\n");
	AuthCmds.push_back("NICK " + nick + "\r\n");
	AuthCmds.push_back("USER " + user + "\r\n");

	for(size_t i = 0; i < AuthCmds.size(); i++)
	{
		if (send(socketBot, AuthCmds[i].c_str(), AuthCmds[i].length(), 0) == -1)
			throw std::runtime_error("failed to send cmd");
	}
}

void	MovieBot::handleMessage(std::string &msg)
{
	std::cout << "[SERVER] " << msg << std::endl;

	 // I should Implement the PING/PONG cmds
	if (msg.compare(0, 4, "PING") == 0)
	{
		std::string pong = "PONG" + msg.substr(4) + "\r\n";
		send(socketBot, pong.c_str(), pong.size(), 0);
		return;
	}

	if (msg.find(" 001 ") != std::string::npos)
	{
		std::string join = "JOIN #movies\r\n";
		send(socketBot, join.c_str(), join.size(), 0);
		return;
	}

	if (msg.find("PRIVMSG") != std::string::npos)
	{
		_handlePrivMsg(msg); return;
	}

	if (msg.compare(0, 5, "ERROR") == 0)
	{
		std::cerr << "Server error: " << msg << std::endl;
		return;
	}
}

void	MovieBot::handleMessage()
{
	size_t pos;

	while (69)
	{
		pos = recieveBuff.find("\r\n");
		size_t len_spec = 2;

		if (pos == std::string::npos)
		{
			pos = recieveBuff.find("\n");
			len_spec = 1;
		}

		if (pos == std::string::npos)
			break;

		std::string fullMsg = recieveBuff.substr(0, pos);
		recieveBuff.erase(0, pos + len_spec);

		if (fullMsg.empty())
			continue;

		processMsg(fullMsg);
	}
}


void    MovieBot::buildBot()
{
	char	buffer[BUFFER_SIZE] = {'\0'};

	while (69)
	{
		ssize_t	bytes = recv(socketBot, buffer, sizeof(buffer) - 1, 0);
		if (bytes <= 0)
		{
			if (bytes == 0)
				std::cerr << "Client disconnected: fd = " << socketBot <<std::endl;
			else
				std::cerr << "recv() error on fd = " << socketBot <<std::endl;
			return;
		}
		recieveBuff += std::string(buffer, bytes);
		this->handleMessage(); // TODO
	}
}

void    MovieBot::executeMovieBot()
{
	this->connectToServer();
	this->buildBot(); // TODO
}