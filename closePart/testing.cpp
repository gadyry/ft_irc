#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <sstream>
# include <sys/socket.h>
# include <poll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <errno.h>

# include "Client.hpp"

# define BUFFER_SIZE 1024

class Server
{
private:
    unsigned short port;
    std::string password;
    int serv_fd;
    std::vector<struct pollfd> fds_sentinels;
    std::map<int, Client*> clients;

    // ✅ REMOVED: Complex typedef and map!
    // typedef void (Server::*handleCommand)(Client*);
    // std::map<std::string, handleCommand> __commands;

    // Methods
    void addClient();
    void removeClient(int fd);
    void processCommand(Client* client, const std::string& cmd);  // ✅ Simple!
    void processCmds(int fd);
    void receiveData(int fdClient);
    void sendError(Client* client, const std::string& msg);
    
    // Command handlers (simple methods)
    void handleNick(Client* client, std::vector<std::string>& params);
    void handleUser(Client* client, std::vector<std::string>& params);
    void handleQuit(Client* client, std::vector<std::string>& params);
    void handleJoin(Client* client, std::vector<std::string>& params);
    void handlePrivmsg(Client* client, std::vector<std::string>& params);
    void handlePing(Client* client, std::vector<std::string>& params);

public:
    Server();
    Server(unsigned short port, std::string password);
    ~Server();

    void executeServ();
};

#endif

// ============= SIMPLE VERSION - Implementation =============

#include "Server.hpp"

// Helper function to split string into tokens
std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void Server::sendError(Client* client, const std::string& msg) {
    std::cout << "Error for client fd " << client->getFd() << ": " << msg << std::endl;
    std::string errorMsg = msg + "\r\n";
    send(client->getFd(), errorMsg.c_str(), errorMsg.length(), 0);
}

// ✅ SIMPLE VERSION - Just if/else!
void Server::processCommand(Client* client, const std::string& fullCmd) {
    if (fullCmd.empty())
        return;
    
    std::vector<std::string> tokens = split(fullCmd);
    
    if (tokens.empty())
        return;
    
    std::string cmd = tokens[0];
    
    // Convert to uppercase for case-insensitive comparison
    for (size_t i = 0; i < cmd.length(); i++)
        cmd[i] = std::toupper(cmd[i]);
    
    // ✅ Simple if/else - easy to understand!
    if (cmd == "NICK") {
        handleNick(client, tokens);
    }
    else if (cmd == "USER") {
        handleUser(client, tokens);
    }
    else if (cmd == "QUIT") {
        handleQuit(client, tokens);
    }
    else if (cmd == "PING") {
        handlePing(client, tokens);
    }
    else if (cmd == "JOIN") {
        // Check if registered
        if (!client->isRegistered()) {
            sendError(client, "451 :You have not registered");
            return;
        }
        handleJoin(client, tokens);
    }
    else if (cmd == "PRIVMSG") {
        if (!client->isRegistered()) {
            sendError(client, "451 :You have not registered");
            return;
        }
        handlePrivmsg(client, tokens);
    }
    else if (cmd == "PART") {
        if (!client->isRegistered()) {
            sendError(client, "451 :You have not registered");
            return;
        }
        // handlePart(client, tokens);
    }
    else {
        // Unknown command
        sendError(client, "421 " + tokens[0] + " :Unknown command");
    }
}

void Server::processCmds(int fd) {
    Client* client = clients[fd];
    
    if (!client) {
        std::cerr << "Error: Client not found for fd " << fd << std::endl;
        return;
    }
    
    std::string& buffer = client->getInputBuffer();
    size_t position;

    while ((position = buffer.find("\r\n")) != std::string::npos) {
        std::string cmd = buffer.substr(0, position);
        buffer.erase(0, position + 2);

        if (!cmd.empty()) {
            processCommand(client, cmd);
            
            // Check if client was deleted (e.g., by QUIT command)
            if (clients.find(fd) == clients.end()) {
                std::cout << "Client disconnected during command processing" << std::endl;
                break;  // Stop processing remaining commands
            }
        }
    }
}

void Server::receiveData(int fdClient) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    
    ssize_t bytes = recv(fdClient, buffer, BUFFER_SIZE - 1, 0);
    
    if (bytes <= 0) {
        if (bytes == 0)
            std::cout << "Client disconnected: fd=" << fdClient << std::endl;
        else
            std::cerr << "recv() error on fd=" << fdClient << std::endl;
        
        removeClient(fdClient);
        return;
    }
    
    Client* client = clients[fdClient];
    if (client) {
        client->appendBuffer(std::string(buffer, bytes));
        processCmds(fdClient);
    }
}

// ============= Example Command Handlers =============

void Server::handleNick(Client* client, std::vector<std::string>& params) {
    if (params.size() < 2) {
        sendError(client, "431 :No nickname given");
        return;
    }
    
    std::string nickname = params[1];
    
    // TODO: Check if nickname is already in use
    // TODO: Validate nickname format
    
    client->setNickname(nickname);
    std::cout << "Client " << client->getFd() << " set nickname to: " << nickname << std::endl;
    
    // Check if now registered (has both NICK and USER)
    // checkRegistration(client);
}

void Server::handleUser(Client* client, std::vector<std::string>& params) {
    if (params.size() < 5) {
        sendError(client, "461 USER :Not enough parameters");
        return;
    }
    
    // USER <username> <mode> <unused> :<realname>
    std::string username = params[1];
    
    client->setUsername(username);
    std::cout << "Client " << client->getFd() << " set username to: " << username << std::endl;
    
    // Check if now registered
    // checkRegistration(client);
}

void Server::handleQuit(Client* client, std::vector<std::string>& params) {
    std::string quitMsg = "Client quit";
    
    if (params.size() > 1) {
        // Get quit message (everything after QUIT)
        quitMsg = params[1];
    }
    
    std::cout << "Client " << client->getFd() << " quit: " << quitMsg << std::endl;
    
    // Send goodbye
    std::string msg = "ERROR :Closing connection\r\n";
    send(client->getFd(), msg.c_str(), msg.length(), 0);
    
    // Remove client
    removeClient(client->getFd());
}

void Server::handlePing(Client* client, std::vector<std::string>& params) {
    if (params.size() < 2) {
        sendError(client, "409 :No origin specified");
        return;
    }
    
    // PONG response
    std::string pong = "PONG :" + params[1] + "\r\n";
    send(client->getFd(), pong.c_str(), pong.length(), 0);
}

void Server::handleJoin(Client* client, std::vector<std::string>& params) {
    if (params.size() < 2) {
        sendError(client, "461 JOIN :Not enough parameters");
        return;
    }
    
    std::string channelName = params[1];
    std::cout << client->getNickname() << " wants to join " << channelName << std::endl;
    
    // TODO: Implement channel join logic
}

void Server::handlePrivmsg(Client* client, std::vector<std::string>& params) {
    if (params.size() < 3) {
        sendError(client, "411 :No recipient given (PRIVMSG)");
        return;
    }
    
    std::string target = params[1];
    std::string message = params[2];
    
    std::cout << client->getNickname() << " -> " << target << ": " << message << std::endl;
    
    // TODO: Implement message sending
}

// ============= Comparison =============

/*
BEFORE (Complex):
    typedef void (Server::*handleCommand)(Client*);
    std::map<std::string, handleCommand> __commands;
    
    // In constructor:
    __commands["NICK"] = &Server::handleNick;
    __commands["USER"] = &Server::handleUser;
    // ...
    
    // When processing:
    (this->*(it->second))(client);  // 🤯 WHAT?!

AFTER (Simple):
    if (cmd == "NICK")
        handleNick(client, tokens);
    else if (cmd == "USER")
        handleUser(client, tokens);
    else if (cmd == "QUIT")
        handleQuit(client, tokens);
    // ...
    
    // ✅ So much clearer!
*/






void Server::_handleCmd(std::string& fullCmd, int fd)
{
    Client* client = clients[fd];
    if (!client)
        return;

    std::istringstream iss(fullCmd);
    std::string cmd;
    iss >> cmd;

    // IRC is case-insensitive
    for (size_t i = 0; i < cmd.size(); i++)
        cmd[i] = std::toupper(cmd[i]);

    if (cmd == "NICK")
        handleNick(*client, iss);
    else if (cmd == "USER")
        handleUser(*client, iss);
    else if (cmd == "JOIN")
        handleJoin(*client, iss);
    else if (cmd == "PRIVMSG")
        handlePrivmsg(*client, iss);
    else if (cmd == "QUIT")
        handleQuit(*client);
    else
        client->sendMsg("421 " + cmd + " :Unknown command\r\n");
}


void Server::_handleCmd(std::string& fullCmd, int fd)
{
    Client* client = clients[fd];
    if (!client)
        return;

    std::vector<std::string> args = split_or(fullCmd);
    if (args.empty())
        return;

    std::string cmd = args[0];

    // IRC commands are case-insensitive
    for (size_t i = 0; i < cmd.size(); ++i)
        cmd[i] = std::toupper(cmd[i]);

    if (cmd == "NICK")
        handleNick(*client, args);
    else if (cmd == "USER")
        handleUser(*client, args);
    else if (cmd == "JOIN")
        handleJoin(*client, args);
    else if (cmd == "PRIVMSG")
        handlePrivmsg(*client, fullCmd); // ⚠ special case
    else if (cmd == "QUIT")
        handleQuit(*client);
    else
        client->sendMsg("421 " + cmd + " :Unknown command\r\n");
}
