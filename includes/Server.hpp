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

# include "Client.hpp"

# define BUFFER_SIZE 1024

class Client;

class Server
{
private :
    u_short       port;
    std::string password;
    int         serv_fd;
    std::vector<struct pollfd> fds_sentinels;
    std::map<int, std::string> clients;

    // methods: 
    void    addClient();

    void    removeClient(int fd);
    void    processCmds(int fd);
    void    recieveData(int fdClient);

public :
    Server();
    Server(u_short port, std::string password);
    ~Server();

    // methods :
    void    executeServ();
};

#endif