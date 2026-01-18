#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>

class Client
{
private :
    int         fd_client;
    std::string nickname;
    std::string username;
    std::string host;
    std::string inputBuffer;
    // bool        reg_stat;
    // ...
public :
    Client();
    ~Client();
    // ...
};

#endif