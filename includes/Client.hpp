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
    std::string inputBuffer;
    bool        reg_stat;
    // ...
public :
    void send(const std::string& msg);
    // ...
};

#endif