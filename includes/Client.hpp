#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "Server.hpp"

# include <string>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <set>

enum AuthState
{
    AUTH_DISCONNECTED,
    AUTH_PASS_OK,
    AUTH_PASS_NICK_OK,
    AUTH_COMPLETE
}

class Client
{
private:
    int                     fd_client;
    std::string             nickname;
    std::string             username;
    std::string             host;
    std::string             inputBuffer;
    AuthState               authState;
    std::set<std::string>   joinedChannels; // chose of set : ignore the duplicate strings!

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

    // Getters
    short       getFdClient();
    std::string getNickname();
    std::string getUsername();
    std::string getHost();
    std::string getInputBuffer();

    // Authentication methods
    void    setPassOk();
    void    setNickOk();
    void    setUserOk();
    bool    checkAuthComplete();

    void clearInputBuffer();
};

#endif