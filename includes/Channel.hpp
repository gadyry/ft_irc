//hello world hh

#pragma once

# include <sys/socket.h>
# include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>
#include "Client.hpp"

class Client;

class Channel
{
    private:
    std::string     ch_name;
    std::string     ch_topic;

    // channel members and shit
    std::vector<Client*>        ch_members;
    std::vector<Client*>        ch_admiins;
    std::vector<std::string>    ch_invited;
    public:
    Channel(std::string name);
    ~Channel();
        
        //getters : 
    std::string ch_getName() const;
    std::string ch_getTopic() const;
    std::string getUserList();

    //JOIN fc
    void addMember(Client *client);
    void removeMember(Client *client);
    bool isMember(Client *client);

    //admiins
    void addadmiin(Client *client);
    void removeadmiin(Client *client);
    bool isadmiin(Client *client);
    /*core channell
        adding/removing members 
    */

    /*
        ivited to channells..
    */
   // broadcast msg in channell
   void broadcast(const std::string& message);
};
