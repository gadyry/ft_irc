#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "Server.hpp"

# include <string>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <set>

class Client
{
private:
    int                     fd_client;
    std::string             nickname;
    std::string             username;
    std::string             host;
    std::string             inputBuffer;
    bool                    reg_stat; // khdamteek db a B9 jatt
    std::set<std::string>   joinedChannels; // chose of set : ignore the duplicate strings!

    bool pass_ok;
    bool nick_ok;
    bool user_ok;


public:
    // Constructor & Destructor
    Client();
    Client(int fd);
    ~Client();

    // Setters
    void setFdClient(short fd);
    void setNickname(const std::string &nick);
    void setUsername(const std::string &user);
    void setHost(const std::string &h);
    void setInputBuffer(const std::string &buf);
    void setRegStat(bool status);

    // Getters
    short       getFdClient();
    std::string getNickname();
    std::string getUsername();
    std::string getHost();
    std::string getInputBuffer();
    bool        getReg_stat();

    void clearInputBuffer();
};

#endif