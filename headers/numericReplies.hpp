#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP

/*----- COMMANDS ERRORS -----*/

# define ERR_NEEDMOREPARAMS(nick, command) (":localhost 461 " + nick + " " + command + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTERED(nick) (":localhost 462 " + nick + " :You may not reregister\r\n")
# define ERR_CHANOPRIVSNEEDED(nick, chan) (":localhost 482 " + nick + " " + chan + " :You're not channel operator\r\n")
# define ERR_NOTONCHANNEL(nick, chan) (":localhost 442 " + nick + " " + chan + " :You're not on that channel\r\n")

// PASS //
# define ERR_PASSWDMISMATCH(nick) (":localhost 464 " + nick + " :Password incorrect\r\n")

// NICK //
# define ERR_NONICKNAMEGIVEN(nick) (":localhost 431 " + nick + " :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(nick, new_nick) (":localhost 432 " + nick + " " + new_nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nick, new_nick) (":localhost 433 " + nick + " " + new_nick + " :Nickname is already in use\r\n")

// JOIN //
# define ERR_BADCHANMASK(channel) (":localhost 476 " + channel + " :Bad channel name\r\n")
# define ERR_INVITEONLYCHAN(nick, channel) (":localhost 473 " + nick + " " + channel + " :You need an invitation to join this channel\r\n")

// INVITE //
# define ERR_USERONCHANNEL(nick, invitedNick, chan) (":localhost 443 " + nick + " " + invitedNick + " " + chan + " :is already on channel\r\n")

/*----- COMMANDS REPLIES -----*/

// JOIN //
# define JOINCHANNEL(nick, channel) (":" + nick + " JOIN " + channel + "\r\n")
# define RPL_TOPIC(nick, channel, topic) (":localhost 332 " + nick + " " + channel + " :" + topic + "\r\n")
# define RPL_NOTOPIC(nick, channel) (":localhost 331 " + nick + " " + channel + " :No topic is set\r\n")
# define RPL_NAMREPLY(nick, symbol, channel) (":localhost 353 " + nick + " " + symbol + channel + " :")
# define RPL_ENDOFNAMES(nick, channel) (":localhost 366 " + nick + " " + channel + " :End of /NAMES list\r\n")

/*----- REPLIES -----*/

# define RPL_WELCOME(nick) (":localhost 001 " + nick + " :Welcome to our IRC server " + nick + " !\r\n")
# define RPL_YOURHOST(nick) (":localhost 002 " + nick + " :Your host is localhost\r\n")
# define RPL_CREATED(nick, time) (":localhost 003 " + nick + " :This server was created " + date + "\r\n")
# define RPL_MYINFO(nick) (":localhost 004 " + nick + " server name: localhost \nAvailable user modes: \n\t-Regular users\n\t-Operators\n \
Available channel modes : \n\t-i: Set/remove Invite-only channel\n\t-t: Set/remove the restrictions of the TOPIC command to channel \
operators\n\t-k: Set/remove the channel key (password)\n\t-o: Give/take channel operator privilege\n\t\
-l: Set/remove the user limit to channel\r\n")

// INVITE //
# define RPL_INVITING(nick, invitedNick, chan) (":localhost 341 " + nick + " invited " + invitedNick + " to " + chan + "\r\n")
# define INVITE(nick, chan) (":" + nick + " invited you to " + chan + "\r\n")

// PRIVMSG //
# define ERR_CANNOTSENDTOCHAN(nick, channel) (":localhost 404 " + nick + " " + channel + " :Cannot send to channel\r\n")
# define ERR_NOSUCHNICK(nick) ("localhost 401 " + nick + ": No such nick/channel\r\n")
# define ERR_NOSUCHSERVER(nick) ("localhost 402 " + nick + " " + "server name" + " :No such server\r\n")
# define ERR_TOOMANYTARGETS(target) ("localhost 407 " + target + ":Duplicate recipients. No message delivered")

// MODE //
# define ERR_NOSUCHCHANNEL(nick, channel) ("localhost 403 " + nick + " " + channel + ": No such channel\r\n")
# define ERR_MODEUNKNOWFLAG(channel, flag) ("localhost 501 " + channel + " :Unknown MODE flag '" + flag + "'\r\n")
# define ERR_WRONGMODEFORMAT(nick, modestring) ("localhost ??? " + nick + " :Wrong format for mode \'" + modestring + "\r\n'")
# define RPL_CHANNELMODEIS(nick, channel) ("localhost 324 " + nick + " " + channel + "\r\n")
# define RPL_CREATIONTIME(nick, channel) ("locahost 329 " + nick + " " + channel + "\r\n")


#endif