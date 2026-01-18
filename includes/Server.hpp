#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>   // sockaddr_in, INADDR_ANY
# include <arpa/inet.h>    // htons, htonl
# include <unistd.h>       // close

// # define    PF_INET IPV4
// # define    SOCK_STREAM TCP_SOCK

class Server
{
private :
    short       port;
    std::string password;
    int         serv_fd;
    // std::map<int fd, Client* cl> clients;

public :
    Server();
    Server(short port, std::string password);
    ~Server();

    // methods :
    void    executeServ();
};

#endif