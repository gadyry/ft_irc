#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <iostream>
# include <map>

class Client
{
private :
    int         fd;
    std::string nickname;
    std::string username;
    std::string host;
    // ...
public :
    void send(const std::string& msg);
    // ...
};

#endif