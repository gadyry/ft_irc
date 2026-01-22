#ifndef CLIENT_HPP
#define CLIENT_HPP

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>

class Client
{
private:
    short       fd_client;
    std::string nickname;
    std::string username;
    std::string host;
    std::string inputBuffer;
    bool        reg_stat;

public:
    // Constructor & Destructor
    Client();
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
