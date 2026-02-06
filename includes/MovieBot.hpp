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
# include <arpa/inet.h>    // htons, htonl
# include <unistd.h>       // close
# include <errno.h>
# include <sstream>        // for istingstream
# include <istream>        // for getline(isstreamstring, dorthwa);

# include "Client.hpp"
# include "IrcReplies.hpp"

class MovieBot
{
private:
    // Attribut :
    int             socketBot;
    std::string     nick;
    std::string     user;
    unsigned short  servPort;
    std::string     password;
    std::string     buffRecieve;

    // methods :
public:
    // Constractor && Distractor:
        MovieBot();
        MovieBot(u_short port, std::string password);
        ~MovieBot();
    
    // Setters && Getters

    // methods helper
    void    executeMovieBot();
};

#endif