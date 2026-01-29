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
    void    _cmdPass(Client* client, std::vector<std::string>& tokens, std::string& fullCmd);
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