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
	format: MODE channelName params User/ChanOp
l: FOR CHANNEL | Set/Remove the user limit to channel.
	Ask mthibaul for his limit of 10 and remove it if unset here.
*/

static std::string getSetModes(std::string modes) {
	static_cast<void>(modes);
	return "";
}

static std::string getRemoveModes(std::string modes) {
	static_cast<void>(modes);
	return "";
}

void	mode(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);

	if (msg.getParams().size() != 2 || msg.getParams().size() != 3) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return ;
	}

	std::string target = msg.getParams()[0];
	std::string modes = msg.getParams()[1];
	std::map<const std::string, Channel> &channels = serv->getChannels();
	std::map<const std::string, Channel>::iterator itChannel = channels.find(target);
	
	if (itChannel != channels.end()) {
		Channel &channel = itChannel->second;
		static_cast<void>(channel);
	}
	else {
		addToClientBuf(serv, clientFd, ERR_NOSUCHCHANNEL(client.getNickname(), msg.getParams()[0]));
	}
	std::string setModes = getSetModes(modes);
	if (setModes.empty()) {
		return ;
	}
	std::string removeModes = getRemoveModes(modes);
	if (removeModes.empty()) {
		return ;
	}
}
