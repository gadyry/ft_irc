#ifndef MOVIEBOT_HPP
#define MOVIEBOT_HPP

# include <iostream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <sys/types.h>
# include <fcntl.h>        // fcntl
# include <poll.h>         // struct pollfd, poll()
# include <netinet/in.h>   // sockaddr_in, INADDR_ANY
# include <arpa/inet.h>    // htons, htonl, inet_aton
# include <unistd.h>       // close
# include <errno.h>
# include <sstream>        // for istingstream
# include <istream>        // for getline

# include "Client.hpp"
# include "IrcReplies.hpp"

class MovieBot
{
private:
    // Attribut :
    int             socketBot;
    std::string     hostname;
    std::string     nick;
    std::string     user;
    unsigned short  servPort;
    std::string     password;
    std::string     buffRecieve;

    // methods :
    void    connectToServer();
public:
    // Constractor && Distractor:
        MovieBot();
        MovieBot(std::string host, u_short port, std::string password);
        ~MovieBot();
    
    // Setters && Getters

    // methods helper
    void    executeMovieBot();
};

#endif