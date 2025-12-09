#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>

class Server
{
private :
    int port;
    std::string password;
    int serv_fd;
    std::map<int fd, Client* cl> clients;
public :
    Server();
    Server(int port, std::string password);
    ~Server();

    
};

#endif