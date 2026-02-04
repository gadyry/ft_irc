#ifndef BOT_HPP
#define BOT_HPP

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

class Bot
{
private:
    // Attribut :

    // methods :
public:
    // Constractor && Distractor:
        Bot();
        Bot(u_short port, std::string password);
        ~Bot();
    
    // Setters && Getters
};

#endif