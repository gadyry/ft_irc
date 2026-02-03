#include "../includes/Channel.hpp"

Channel::Channel(std::string name) : ch_name(name), ch_topic("") {}

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

void Channel::broadcast(const std::string& message) {
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

Channel::~Channel() {}