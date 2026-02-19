#pragma once

# include <sys/socket.h>
# include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>
# include <algorithm>
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

    std::string ch_getName() const;
    std::string ch_getTopic() const;
    std::string getUserList();
    Client* getMemberName(std::string nick);
    const std::vector<Client*>& getAdmins() const;
    std::string getChFlags();
    Client* getFirstMember();

    bool        isInviteOnly() const;
    bool        isTopicAdmOnly() const;
    std::string getPassKey() const;
    size_t      getLimitUser() const;
    size_t      getMemSize() const;

    void setInviteOnly(bool val);
    void setTopicAdmOnly(bool val);
    void setPassKey(std::string key);
    void setLimitUser(size_t limit);
    void setTopic(std::string topic);

    void addInvite(std::string nick);
    bool isInvited(std::string nick);
    void removeInvite(std::string nick);

    void addMember(Client *client);
    void removeMember(Client *client);
    bool isMember(Client *client);

    void addadmiin(Client *client);
    void removeadmiin(Client *client);
    bool isadmiin(Client *client);

    void    broadcast(const std::string& message);
    void    sendToMembers(Client* client, std::string& msg);
};
