//hello world hh

#pragma once

# include <sys/socket.h>
# include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>
# include <algorithm> // for std::find(...) overriding we need it in linux !!
#include "Client.hpp"

class Client;

class Channel
{
    private:
    std::string     ch_name;
    std::string     ch_topic;

    // channel members
    std::vector<Client*>        ch_members;
    std::vector<Client*>        ch_admiins;
    std::vector<std::string>    ch_invited;

    // modes
    bool        _inviteOnly;
    bool        _topicAdmOnly;
    std::string _passKey;
    size_t      _limitUser;

    public:
    Channel(std::string name);
    ~Channel();
        
        //getters : 
    std::string ch_getName() const;
    std::string ch_getTopic() const;
    std::string getUserList();
    Client* getMemberName(std::string nick);

    //modes
    std::string getChFlags();

    // Getters for Join logic checks
    bool        isInviteOnly() const;
    bool        isTopicAdmOnly() const;
    std::string getPassKey() const;
    size_t      getLimitUser() const;

    // Setters for Mode command
    void setInviteOnly(bool val);
    void setTopicAdmOnly(bool val);
    void setPassKey(std::string key);
    void setLimitUser(size_t limit);
    void setTopic(std::string topic);
    
    // Invite logic (needed for +i)
    void addInvite(std::string nick);
    bool isInvited(std::string nick);
    void removeInvite(std::string nick);


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


    void    sendToMembers(Client* client, std::string& msg);
};
