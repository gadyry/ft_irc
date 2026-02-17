#include "../includes/Channel.hpp"

Channel::Channel(std::string name) : ch_name(name), ch_topic(""),
        _inviteOnly(false), _topicAdmOnly(true), _passKey(""),
        _limitUser(0) {
}

std::string Channel::ch_getName() const {
    return ch_name;
}

std::string Channel::ch_getTopic() const {
    return ch_topic;
}

bool Channel::isMember(Client *client) {
    std::vector<Client*>::iterator it = std::find(ch_members.begin(), ch_members.end(), client);
    return (it != ch_members.end());
}

void Channel::addMember(Client *client) {
    if (isMember(client))
        return ;
    ch_members.push_back(client);
    client->addChannel(ch_name);
    if (ch_members.size() == 1) {
        addadmiin(client);
    }
}

void Channel::removeMember(Client *client) {
    std::vector<Client*>::iterator it = std::find(ch_members.begin(), ch_members.end(), client);
    if (it != ch_members.end()) {
        ch_members.erase(it);
        client->removeChannel(ch_name); 
    }
    if (isadmiin(client)) {
        removeadmiin(client);
    }
}

void Channel::addadmiin(Client *client) {
    if (!isadmiin(client))
        ch_admiins.push_back(client);
}

void Channel::removeadmiin(Client *client) {
    std::vector<Client*>::iterator it = std::find(ch_admiins.begin(), ch_admiins.end(), client);
    if (it != ch_admiins.end())
        ch_admiins.erase(it);
}

bool Channel::isadmiin(Client *client) {
    std::vector<Client*>::iterator it = std::find(ch_admiins.begin(), ch_admiins.end(), client);
    return (it != ch_admiins.end());
}

void    Channel::broadcast(const std::string& message) {
    for (size_t i = 0; i < ch_members.size(); i++) {
        if (ch_members[i]) {
            send(ch_members[i]->getFdClient(), message.c_str(), message.length(), 0);
        }
    }
}

std::string Channel::getUserList() {
    std::string list = "";
    for (size_t i = 0; i < ch_members.size(); i++) {
        if (isadmiin(ch_members[i]))
            list += "@";
        list += ch_members[i]->getNickname();
        if (i < ch_members.size() - 1)
            list += " ";
    }
    return list;
}

Client* Channel::getMemberName(std::string nick) {
    for (size_t i = 0; i < ch_members.size(); i++) {
        if (ch_members[i]->getNickname() == nick)
            return ch_members[i];
    }
    return NULL;
}

size_t  Channel::getMemSize() const {
    return ch_members.size();
}

bool Channel::isInviteOnly() const {
    return _inviteOnly;
}

bool Channel::isTopicAdmOnly() const {
    return _topicAdmOnly;
}

std::string Channel::getPassKey() const {
    return _passKey;
}

size_t Channel::getLimitUser() const {
    return _limitUser;
}

void Channel::setInviteOnly(bool val) {
    _inviteOnly = val;
}

void Channel::setTopicAdmOnly(bool val) {
    _topicAdmOnly = val;
}

void Channel::setPassKey(std::string key) {
    _passKey = key;
}

void Channel::setLimitUser(size_t limit) {
    _limitUser = limit;
}

void Channel::setTopic(std::string topic) {
    ch_topic = topic;
}

bool Channel::isInvited(std::string nick) {
    for (size_t i = 0; i < ch_invited.size(); i++) {
        if (ch_invited[i] == nick)
            return true;
    }
    return false;
}

void Channel::addInvite(std::string nick) {
    if (!isInvited(nick))
        ch_invited.push_back(nick);
}

void Channel::removeInvite(std::string nick) {
    for (std::vector<std::string>::iterator it = ch_invited.begin(); it != ch_invited.end(); ++it) {
        if (*it == nick) {
            ch_invited.erase(it);
            break;
        }
    }
}

std::string Channel::getChFlags() {
    std::string modeFlag = "+";
    if (_inviteOnly)
        modeFlag += "i";
    if (_topicAdmOnly)
        modeFlag += "t";
    if (!_passKey.empty())
        modeFlag += "k";
    if (_limitUser > 0)
        modeFlag += "l";
    return modeFlag;
}

void    Channel::sendToMembers(Client* sender, std::string& msg)
{
    for(size_t i = 0; i < ch_members.size(); i++)
    {
        if (ch_members[i] && ch_members[i] != sender)
            send(ch_members[i]->getFdClient(), msg.c_str(), msg.length(), 0);
    }
}

Channel::~Channel() {}