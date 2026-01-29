#ifndef IRC_REPLIES_HPP
#define IRC_REPLIES_HPP

/* ===== ERRORS ===== */

// Registration
#define ERR_NOTREGISTERED      "451"
#define ERR_NEEDMOREPARAMS    "461"
#define ERR_ALREADYREGISTRED  "462"
#define ERR_PASSWDMISMATCH    "464"

// Nickname
#define ERR_NONICKNAMEGIVEN   "431"
#define ERR_ERRONEUSNICKNAME  "432"
#define ERR_NICKNAMEINUSE     "433"

// Commands
#define ERR_UNKNOWNCOMMAND    "421"

// Messaging
#define ERR_NOSUCHNICK        "401"
#define ERR_NORECIPIENT       "411"
#define ERR_NOTEXTTOSEND     "412"

// Channels
#define ERR_NOSUCHCHANNEL     "403"
#define ERR_NOTONCHANNEL     "442"
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_USERONCHANNEL    "443"

// Channel modes (optional)
#define ERR_CHANNELISFULL    "471"
#define ERR_INVITEONLYCHAN   "473"
#define ERR_BADCHANNELKEY    "475"

/* ===== REPLIES ===== */

#define RPL_WELCOME  "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED  "003"
#define RPL_MYINFO   "004"

#endif
