# include "../includes/MovieBot.hpp"
# include "../includes/Server.hpp"

MovieBot::MovieBot() : socketBot(-1), hostname(""), nick(""), user(""), servPort(6969),
					password(""), recieveBuff("")
{std::srand(std::time(NULL));}

MovieBot::MovieBot(std::string host, u_short port, std::string password) : socketBot(-1), hostname(host), nick("MovieBot"), user("moviebot cretical cinephel :CelimaBot"),
					servPort(port), password(password), recieveBuff("")
{ std::srand(std::time(NULL));}

MovieBot::~MovieBot()
{
	if (this->socketBot >= 0)
	{
		close(this->socketBot);
		this->socketBot = -1;
	}
}

void	MovieBot::sendPrivMsg(std::string& target, std::string& message)
{
	std::string reply = "PRIVMSG " + target + " :" + message + "\r\n";

	if (send(socketBot, reply.c_str(), reply.size(), 0) < 0)
		LOG(ERROR, "send() failed in sendPrivMsg()");
}

// methods 
void	MovieBot::connectToServer()
{
	struct sockaddr_in	addr_serv;
	int					camus = 1;

	this->socketBot = socket(PF_INET, SOCK_STREAM, 0);
	if (socketBot == -1)
		throw std::runtime_error("failed to create socket!");

	if (setsockopt(socketBot, SOL_SOCKET, SO_REUSEADDR, &camus, sizeof(camus)) == -1)
	{
		close(socketBot);
		socketBot = -1;
		throw std::runtime_error("setsockopt() failed!");
	}

	addr_serv.sin_family = PF_INET;
	addr_serv.sin_port = htons(this->servPort);
	
	if (inet_pton(AF_INET, this->hostname.c_str(), &addr_serv.sin_addr) <= 0)
		throw std::runtime_error("Invalid hostname/IP!");

	if (connect(this->socketBot, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)
	{
		close(socketBot);
		socketBot = -1;
		throw std::runtime_error("connect() failed!");
	}

	std::vector<std::string> AuthCmds;
	AuthCmds.push_back("PASS " + password + "\r\n");
	AuthCmds.push_back("NICK " + nick + "\r\n");
	AuthCmds.push_back("USER " + user + "\r\n");

	for(size_t i = 0; i < AuthCmds.size(); i++)
	{
		if (send(socketBot, AuthCmds[i].c_str(), AuthCmds[i].length(), 0) == -1)
			LOG(ERROR, "failed to send cmd");
	}
}

void	MovieBot::dealWithPrivMsg(std::string& prefix, std::vector<std::string>& args)
{
	if (args.size() < 2) return;

	std::string	target = args[0];
	std::string	msgContent = args[1];

	if (msgContent.empty()) return;

	size_t pos = prefix.find('!');
	std::string sender = (pos != std::string::npos) ? prefix.substr(0, pos) : prefix;
	if (sender == this->nick)
		return;
	if (msgContent[0] == '\x01') return;

	std::string replyTarget;
	if (target == this->nick)
		replyTarget = sender;
	else if (!target.empty() && target[0] == '#')
		replyTarget = target;
	else
		return;

	if (msgContent[0] != '!') return;

	std::string	cmdLine = msgContent.substr(1);
	std::istringstream isstream(cmdLine);
	std::string cmd; isstream >> cmd;
	for (size_t i = 0; i < cmd.length(); i++)
		cmd[i] = std::tolower(cmd[i]);
	std::vector<std::string> cmdArgs;
	std::string arg;
	while (isstream >> arg)	cmdArgs.push_back(arg);

	std::string response;
	if (cmd == "quote")
		response = handleQuote(cmdArgs);
	else if (cmd == "help")
		response = handleHelp();
	else if (cmd == "suggest" || cmd == "recommend")
		response = handleSuggest();
	// else if (cmd == "add")
	// 	response = handleAdd(cmdArgs, sender);
	else if (cmd == "info")
		response = handleInfo(cmdArgs);
	else
		response = "❌ Unknown command: " + cmd + ". Try !help";

	if (!response.empty())
	{

		size_t pos = 0;
		std::string line;
		while (pos < response.length())
		{
			size_t newlinePos = response.find("\r\n", pos);
			if (newlinePos == std::string::npos)
			{
				line = response.substr(pos);
				pos = response.length();
			}
			else
			{
				line = response.substr(pos, newlinePos - pos);
				pos = newlinePos + 2;
			}
			if (!line.empty())
				sendPrivMsg(replyTarget, line);
		}
	}
}

void	MovieBot::processMsg(std::string &msg)
{
	std::cout << "[SERVER] " << msg << std::endl;

	if (msg.length() >= 2 && msg.substr(msg.length() - 2) == "\r\n")
		msg = msg.substr(0, msg.length() - 2);

	if (msg.compare(0, 4, "PING") == 0)
	{
		std::string token = "";
		size_t spacePos = msg.find(' ');
		if (spacePos != std::string::npos)
		{
			token = msg.substr(spacePos + 1);
			if (!token.empty() && token[0] == ':')
				token = token.substr(1);
		}
		std::string pongMsg = RPL_PONG(this->hostname, token);
		if (send(socketBot, pongMsg.c_str(), pongMsg.length(), 0) < 0)
			LOG(ERROR, "send() failed for PONG");

		LOG(BOT, "Replied with: " << pongMsg);
		return;
	}

	if (msg.find(" 001 ") != std::string::npos)
	{
		std::string join = "JOIN #movies\r\n";
		if (send(socketBot, join.c_str(), join.size(), 0) < 0)
			LOG(ERROR, "send() failed");
		return;
	}

	std::string	prefix, cmd;
	std::string	line = msg;
	std::vector<std::string> args;
	if (!line.empty() && line[0] == ':')
	{
		size_t pos_space = line.find(' ');
		if (pos_space == std::string::npos)
			return;
		prefix = line.substr(1, pos_space - 1);
		line = line.substr(pos_space + 1);
	}
	size_t	spacePos = line.find(' ');
	if (spacePos == std::string::npos)
	{
		cmd = line;
		line = "";
	}
	else
	{
		cmd = line.substr(0, spacePos);
		line = line.substr(spacePos + 1);
	}
	while (!line.empty())
	{
		if (line[0] == ':')
		{
			args.push_back(line.substr(1));
			break;
		}
		size_t next_space = line.find(' ');
		if (next_space != std::string::npos)
		{
			args.push_back(line.substr(0, next_space));
			line = line.substr(next_space + 1);
		}
		else
		{
			args.push_back(line); break;
		}
	}
	if (cmd == "PRIVMSG")
	{
		dealWithPrivMsg(prefix, args); return;
	}

	if (msg.compare(0, 5, "ERROR") == 0)
	{
		LOG(ERROR, "Server error: " << msg);
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
		this->handleMessage();
	}
}

void	MovieBot::executeMovieBot()
{
	this->connectToServer();

	loadMovies("data/movies.csv");
	loadQuotes("data/quotes.csv");

	this->buildBot();
}