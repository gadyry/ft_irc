#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>

class Server
{
private :
    u_short port;
    std::string password;
    int serv_fd;
    // std::map<int fd, Client* cl> clients;
    
public :
    Server();
    Server(u_short port, std::string password);
    ~Server();

    // methods :
    void    intializeServ();
    void    createServShocket();
};

#endif