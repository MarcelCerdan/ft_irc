#include "main.hpp"

/*
used to set or remove options from a given target.

CHANNEL MODE
	if target is a channel don't know -> ERR_NOSUCHCHANNEL
	if mode without params, RPL_CHANNELMODEIS which give mode infos of the channel. Don't have to show password.
	Also return RPL_CREATIONTIME just after.

+ = Set
- = Remove
i: FOR CHANNEL | Set/Remove Invite-only channel. If set, users can't just join a channel by typing their names.
	In the JOIN function, check if channel has invite-only and refuse to join if it's set.

t: FOR CHANNEL | Set/Remove the restrictions of the TOPIC command to channel operators. If set, only channel operators can
change the TOPIC, otherwise everyone in the channel can do it.
	In the TOPIC function, if a non-operator want to change it, refuse if t is set.
k: FOR CHANNEL | Set/Remove the channel key(password). If Set, it require a password to join a channel.
	Add a password string in Channel class.
	If set, ask a password and fill the string with it.
	In the JOIN function, check if password is empty. If not, ask the password and refuse to join if it's not
	the good one.
o: FOR USER | Give/Take channel operator privilege to someone.
	When set, move a user from the member map to the operator map. If unset, do the opposite.
l: FOR CHANNEL | Set/Remove the user limit to channel.
	Ask mthibaul for his limit of 10 and remove it if unset here.
*/

void	mode(Server *serv, Message msg, int clientFd) {
	static_cast<void>(serv);
	static_cast<void>(clientFd);
	std::cout << "prefix: " << msg.getPrefix() << std::endl;
	for (size_t i = 0; i < msg.getParams().size(); i++)
		std::cout << "params: " << msg.getParams()[i] << std::endl;
}