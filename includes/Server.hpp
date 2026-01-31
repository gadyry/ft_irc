#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <sys/types.h>
# include <fcntl.h>        // fcntl
# include <poll.h>         // struct pollfd, poll()
# include <netinet/in.h>   // sockaddr_in, INADDR_ANY
# include <arpa/inet.h>    // htons, htonl
# include <unistd.h>       // close
# include <errno.h>
# include <sstream>        // for istingstream
# include <istream> // for getline(isstreamstring, dorthwa);

# include "Client.hpp"
# include "IrcReplies.hpp"

// ANSI color macros
# define C_RESET   "\033[0m"
# define C_BOLD    "\033[1m"
# define C_RED     "\033[31m"
# define C_GREEN   "\033[32m"
# define C_YELLOW  "\033[33m"
# define C_BLUE    "\033[34m"
# define C_MAGENTA "\033[35m"
# define C_CYAN    "\033[36m"

# define LOG_LABEL_SERVER C_GREEN C_BOLD "[SERVER] " C_RESET

enum LogLevel
{
    INFO,
    WARN,
    ERROR,
    DEBUG,
    CLIENT,
    NEWCLIENT,
    DISCONNECT
};

void log(LogLevel level, const std::string &msg);

# define LOG(level, msg) do { std::ostringstream ss; ss << msg; log(level, ss.str()); } while(0)

# define BUFFER_SIZE 1024

class Client;

class Server
{
private :
    u_short       port;
    std::string password;
    int         serv_fd;
    std::vector<struct pollfd> fds_sentinels;
    std::map<int, Client*> clients; // for me the best choise using object not std::string

    // typedef void (Server::*handleCommand)(Client*);
    // std::map<std::string, handleCommand> __commands; // the best & clean way to handle cmds , <<if, else == prostutation>>

    // methods: 
    void    addClient();
    void    recieveData(int fdClient);

    void    removeClient(int fd);
    void    sendError(Client* client, const std::string& msg);

    // utils handle cmd part:
    void    _regestrationIsValid(Client* client);
    void    _handleLine(Client* client, std::string& fullCmd);
    void    _handleCmd(Client* client, std::vector<std::string>& tokens);
    void    processCmds(int fd);

    // irc commands:
    void    _cmdPass(Client* client, std::vector<std::string>& tokens);
    void    _cmdNick(Client* client, std::vector<std::string>& tokens);
    void    _cmdUser(Client* client, std::vector<std::string>& tokens);

    void    _cmdJoin(Client* client, std::vector<std::string>& tokens);
    void    _handlePrivmsg(Client* client, std::string fullCmd);

    void    _handleQuit(Client* client);

public :
    Server();
    Server(u_short port, std::string password);
    ~Server();

    // methods :
    void    executeServ();
};

#endif