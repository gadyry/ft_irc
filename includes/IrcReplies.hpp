// ============= RFC 1459 COMPLIANT IRC REPLIES =============
#ifndef IRCREPLIES_HPP
#define IRCREPLIES_HPP

#include <string>

// ============= Base Components =============
#define SERVERPREFIX ":IRCServer"
#define POSTFIX "\r\n"

// ============= CONNECTION REGISTRATION (001-004) =============

// 001 RPL_WELCOME
#define RPL_WELCOME(nick) \
    (std::string(SERVERPREFIX) + " 001 " + (nick) + " :Welcome to the Internet Relay Network " + (nick) + POSTFIX)

// 002 RPL_YOURHOST
#define RPL_YOURHOST(nick, servername, version) \
    (std::string(SERVERPREFIX) + " 002 " + (nick) + " :Your host is " + (servername) + ", running version " + (version) + POSTFIX)

// 003 RPL_CREATED
#define RPL_CREATED(nick, date) \
    (std::string(SERVERPREFIX) + " 003 " + (nick) + " :This server was created " + (date) + POSTFIX)

// 004 RPL_MYINFO
#define RPL_MYINFO(nick, servername, version, usermodes, chanmodes) \
    (std::string(SERVERPREFIX) + " 004 " + (nick) + " " + (servername) + " " + (version) + " " + (usermodes) + " " + (chanmodes) + POSTFIX)

// ============= COMMAND RESPONSES (200-399) =============

// 221 RPL_UMODEIS
#define RPL_UMODEIS(nick, modes) \
    (std::string(SERVERPREFIX) + " 221 " + (nick) + " " + (modes) + POSTFIX)

// 324 RPL_CHANNELMODEIS
#define RPL_CHANNELMODEIS(nick, channel, modes) \
    (std::string(SERVERPREFIX) + " 324 " + (nick) + " " + (channel) + " " + (modes) + POSTFIX)

// 331 RPL_NOTOPIC
#define RPL_NOTOPIC(nick, channel) \
    (std::string(SERVERPREFIX) + " 331 " + (nick) + " " + (channel) + " :No topic is set" + POSTFIX)

// 332 RPL_TOPIC
#define RPL_TOPIC(nick, channel, topic) \
    (std::string(SERVERPREFIX) + " 332 " + (nick) + " " + (channel) + " :" + (topic) + POSTFIX)

// 341 RPL_INVITING
#define RPL_INVITING(nick, target, channel) \
    (std::string(SERVERPREFIX) + " 341 " + (nick) + " " + (target) + " " + (channel) + POSTFIX)

// 353 RPL_NAMREPLY
#define RPL_NAMREPLY(nick, symbol, channel, users) \
    (std::string(SERVERPREFIX) + " 353 " + (nick) + " " + (symbol) + " " + (channel) + " :" + (users) + POSTFIX)

// 366 RPL_ENDOFNAMES
#define RPL_ENDOFNAMES(nick, channel) \
    (std::string(SERVERPREFIX) + " 366 " + (nick) + " " + (channel) + " :End of /NAMES list" + POSTFIX)

// 375 RPL_MOTDSTART
#define RPL_MOTDSTART(nick, server) \
    (std::string(SERVERPREFIX) + " 375 " + (nick) + " :- " + (server) + " Message of the day - " + POSTFIX)

// 372 RPL_MOTD
#define RPL_MOTD(nick, text) \
    (std::string(SERVERPREFIX) + " 372 " + (nick) + " :- " + (text) + POSTFIX)

// 376 RPL_ENDOFMOTD
#define RPL_ENDOFMOTD(nick) \
    (std::string(SERVERPREFIX) + " 376 " + (nick) + " :End of /MOTD command" + POSTFIX)

// ============= ERROR REPLIES (400-599) =============

// 401 ERR_NOSUCHNICK
#define ERR_NOSUCHNICK(nick, target) \
    (std::string(SERVERPREFIX) + " 401 " + (nick) + " " + (target) + " :No such nick/channel" + POSTFIX)

// 403 ERR_NOSUCHCHANNEL
#define ERR_NOSUCHCHANNEL(nick, channel) \
    (std::string(SERVERPREFIX) + " 403 " + (nick) + " " + (channel) + " :No such channel" + POSTFIX)

// 404 ERR_CANNOTSENDTOCHAN
#define ERR_CANNOTSENDTOCHAN(nick, channel) \
    (std::string(SERVERPREFIX) + " 404 " + (nick) + " " + (channel) + " :Cannot send to channel" + POSTFIX)

// 405 ERR_TOOMANYCHANNELS
#define ERR_TOOMANYCHANNELS(nick, channel) \
    (std::string(SERVERPREFIX) + " 405 " + (nick) + " " + (channel) + " :You have joined too many channels" + POSTFIX)

// 411 ERR_NORECIPIENT
#define ERR_NORECIPIENT(nick, command) \
    (std::string(SERVERPREFIX) + " 411 " + (nick) + " :No recipient given (" + (command) + ")" + POSTFIX)

// 412 ERR_NOTEXTTOSEND (RFC 1459: no nick parameter!)
#define ERR_NOTEXTTOSEND(nick) \
    (std::string(SERVERPREFIX) + " 412 " + (nick) + " :No text to send" + POSTFIX)

// 421 ERR_UNKNOWNCOMMAND
#define ERR_UNKNOWNCOMMAND(nick, command) \
    (std::string(SERVERPREFIX) + " 421 " + (nick) + " " + (command) + " :Unknown command" + POSTFIX)

// 422 ERR_NOMOTD
#define ERR_NOMOTD(nick) \
    (std::string(SERVERPREFIX) + " 422 " + (nick) + " :MOTD File is missing" + POSTFIX)

// 431 ERR_NONICKNAMEGIVEN
#define ERR_NONICKNAMEGIVEN(nick) \
    (std::string(SERVERPREFIX) + " 431 " + (nick) + " :No nickname given" + POSTFIX)

// 432 ERR_ERRONEUSNICKNAME
#define ERR_ERRONEUSNICKNAME(nick, badnick) \
    (std::string(SERVERPREFIX) + " 432 " + (nick) + " " + (badnick) + " :Erroneous nickname" + POSTFIX)

// 433 ERR_NICKNAMEINUSE
#define ERR_NICKNAMEINUSE(nick, target) \
    (std::string(SERVERPREFIX) + " 433 " + (nick) + " " + (target) + " :Nickname is already in use" + POSTFIX)

// 436 ERR_NICKCOLLISION
#define ERR_NICKCOLLISION(nick, target) \
    (std::string(SERVERPREFIX) + " 436 " + (nick) + " " + (target) + " :Nickname collision KILL" + POSTFIX)

// 441 ERR_USERNOTINCHANNEL
#define ERR_USERNOTINCHANNEL(nick, target, channel) \
    (std::string(SERVERPREFIX) + " 441 " + (nick) + " " + (target) + " " + (channel) + " :They aren't on that channel" + POSTFIX)

// 442 ERR_NOTONCHANNEL
#define ERR_NOTONCHANNEL(nick, channel) \
    (std::string(SERVERPREFIX) + " 442 " + (nick) + " " + (channel) + " :You're not on that channel" + POSTFIX)

// 443 ERR_USERONCHANNEL
#define ERR_USERONCHANNEL(nick, target, channel) \
    (std::string(SERVERPREFIX) + " 443 " + (nick) + " " + (target) + " " + (channel) + " :is already on channel" + POSTFIX)

// 451 ERR_NOTREGISTERED (RFC 1459: use * for nick since not registered!)
#define ERR_NOTREGISTERED() \
    (std::string(SERVERPREFIX) + " 451 * :You have not registered" + POSTFIX)

// 461 ERR_NEEDMOREPARAMS
#define ERR_NEEDMOREPARAMS(nick, command) \
    (std::string(SERVERPREFIX) + " 461 " + (nick) + " " + (command) + " :Not enough parameters" + POSTFIX)

// 462 ERR_ALREADYREGISTRED
#define ERR_ALREADYREGISTRED(nick) \
    (std::string(SERVERPREFIX) + " 462 " + (nick) + " :You may not reregister" + POSTFIX)

// 464 ERR_PASSWDMISMATCH
#define ERR_PASSWDMISMATCH(nick) \
    (std::string(SERVERPREFIX) + " 464 " + (nick) + " :Password incorrect" + POSTFIX)

// 467 ERR_KEYSET
#define ERR_KEYSET(nick, channel) \
    (std::string(SERVERPREFIX) + " 467 " + (nick) + " " + (channel) + " :Channel key already set" + POSTFIX)

// 471 ERR_CHANNELISFULL
#define ERR_CHANNELISFULL(nick, channel) \
    (std::string(SERVERPREFIX) + " 471 " + (nick) + " " + (channel) + " :Cannot join channel (+l)" + POSTFIX)

// 473 ERR_INVITEONLYCHAN
#define ERR_INVITEONLYCHAN(nick, channel) \
    (std::string(SERVERPREFIX) + " 473 " + (nick) + " " + (channel) + " :Cannot join channel (+i)" + POSTFIX)

// 475 ERR_BADCHANNELKEY
#define ERR_BADCHANNELKEY(nick, channel) \
    (std::string(SERVERPREFIX) + " 475 " + (nick) + " " + (channel) + " :Cannot join channel (+k)" + POSTFIX)

// 476 ERR_BADCHANMASK
#define ERR_BADCHANMASK(channel) \
    (std::string(SERVERPREFIX) + " 476 " + (channel) + " :Bad Channel Mask" + POSTFIX)

// 481 ERR_NOPRIVILEGES
#define ERR_NOPRIVILEGES(nick) \
    (std::string(SERVERPREFIX) + " 481 " + (nick) + " :Permission Denied- You're not an IRC operator" + POSTFIX)

// 482 ERR_CHANOPRIVSNEEDED
#define ERR_CHANOPRIVSNEEDED(nick, channel) \
    (std::string(SERVERPREFIX) + " 482 " + (nick) + " " + (channel) + " :You're not channel operator" + POSTFIX)

// ============= COMMAND REPLIES (Client-to-Client) =============
// Format: :<nick>!<user>@<host> COMMAND <params>

// JOIN
#define RPL_JOIN(prefix, channel) \
    (std::string(":") + (prefix) + " JOIN " + (channel) + POSTFIX)

// PART
#define RPL_PART(prefix, channel, reason) \
    (std::string(":") + (prefix) + " PART " + (channel) + " :" + (reason) + POSTFIX)

// NICK
#define RPL_NICK(prefix, newnick) \
    (std::string(":") + (prefix) + " NICK " + (newnick) + POSTFIX)

// QUIT
#define RPL_QUIT(prefix, reason) \
    (std::string(":") + (prefix) + " QUIT :Quit: " + (reason) + POSTFIX)

// KICK
#define RPL_KICK(prefix, channel, target, reason) \
    (std::string(":") + (prefix) + " KICK " + (channel) + " " + (target) + " :" + (reason) + POSTFIX)

// MODE
#define RPL_MODE(prefix, target, modes, params) \
    (std::string(":") + (prefix) + " MODE " + (target) + " " + (modes) + " " + (params) + POSTFIX)

// INVITE
#define RPL_INVITE(prefix, target, channel) \
    (std::string(":") + (prefix) + " INVITE " + (target) + " " + (channel) + POSTFIX)

// PRIVMSG
#define RPL_PRIVMSG(prefix, target, msg) \
    (std::string(":") + (prefix) + " PRIVMSG " + (target) + " :" + (msg) + POSTFIX)

// NOTICE
#define RPL_NOTICE(prefix, target, msg) \
    (std::string(":") + (prefix) + " NOTICE " + (target) + " :" + (msg) + POSTFIX)

// TOPIC
#define RPL_TOPICCHANGE(prefix, channel, topic) \
    (std::string(":") + (prefix) + " TOPIC " + (channel) + " :" + (topic) + POSTFIX)

// PONG
#define RPL_PONG(server, token) \
    (std::string(":") + (server) + " PONG " + (server) + " :" + (token) + POSTFIX)

// ============= HELPER MACROS =============

// Create client prefix: nick!user@host
#define CLIENT_PREFIX(nick, user, host) \
    ((nick) + "!" + (user) + "@" + (host))

// Asterisk for unregistered clients
#define UNREGISTERED_NICK "*"

#endif // IRCREPLIES_HPP