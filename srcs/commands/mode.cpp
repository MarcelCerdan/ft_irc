#include "main.hpp"
#include <sstream>

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

bool isNumeric(const std::string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit(str[i])) {
            return (false);
        }
    }
    return (true);
}

static std::string getSetModes(std::string modes, Server *serv, int clientFd, std::string channel) {
	std::string modesToSet;
	size_t i = 0;
	bool alreadyMinus = false;

	if (modes[0] == '-') {
		alreadyMinus = true;
		while (i < modes.size() && modes[i] != '+')
			i++;
	}
	if (modes[i] == '+') {
		i++;
		while (i < modes.size()) {
			if (isalpha(modes[i]))
				modesToSet += modes[i];
			else if (modes[i] == '-') {
				if (alreadyMinus)
					addToClientBuf(serv, clientFd, ERR_MODEUNKNOWFLAG(channel, modes[i]));
				else
					break ;
			}
			else
				addToClientBuf(serv, clientFd, ERR_MODEUNKNOWFLAG(channel, modes[i]));
			i++;
		}
	}
	return (modesToSet);
}

static std::string getRemoveModes(std::string modes, Server *serv, int clientFd, std::string channel) {
	std::string modesToRemove;
	size_t i = 0;
	bool alreadyPlus = false;

	if (modes[0] == '+') {
		alreadyPlus = true;
		while ( i < modes.size() && modes[i] != '-')
			i++;
	}
	if (modes[i] == '-') {
		i++;
		while (i < modes.size()) {
			if (isalpha(modes[i]))
				modesToRemove += modes[i];
			else if (modes[i] == '+') {
				if (alreadyPlus)
					addToClientBuf(serv, clientFd, ERR_MODEUNKNOWFLAG(channel, modes[i]));
				else
					break ;
			}
			else
				addToClientBuf(serv, clientFd, ERR_MODEUNKNOWFLAG(channel, modes[i]));
			i++;
		}
	}
	return (modesToRemove);
}

static void applySetModes(std::string setModes, Channel channel, Message msg, int *paramToUse, Server *serv, int clientFd, std::string nick) {
	for (size_t i = 0; i != setModes.size(); i++) {
		if (setModes[i] == 'i')
			channel.setMode(e_i, 1);
		else if (setModes[i] == 't')
			channel.setMode(e_t, 1);
		else if (setModes[i] == 'k') {
			if (static_cast<size_t>(*paramToUse) < msg.getParams().size()) {
				channel.setMode(e_k, 1);
				channel.setPassword(msg.getParams()[*paramToUse]);
				*paramToUse += 1;
			}
			else
				addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(nick, "MODE"));
		}
		else if (setModes[i] == 'l') {
			std::cout << "get max user before everything " << channel.getMaxUsers() << std::endl;
			if (static_cast<size_t>(*paramToUse) < msg.getParams().size()) {
				std::cout << "param: " << msg.getParams()[*paramToUse] << std::endl;
				if (isNumeric(msg.getParams()[*paramToUse])) {
					std::cout << "before atoi" << std::endl;
					int newMaxUser = atoi(msg.getParams()[*paramToUse].c_str());
					std::cout << "after atoi, newMaxUser: " << newMaxUser << std::endl;
					if (newMaxUser <= MAX_CLIENTS) {
						channel.setMode(e_l, 1);
						channel.setMaxUsers(newMaxUser);
						*paramToUse += 1;
					}
					else {
						std::ostringstream oss;
						oss << MAX_CLIENTS;
						std::string maxClientsStr = oss.str();
						addToClientBuf(serv, clientFd, ERR_TOOMANYMEMBERS(nick, channel.getName(), maxClientsStr));
					}
				}
			}
			else
				addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(nick, "MODE"));
		}
		else if (setModes[i] == 'o') {

		}
	}
}

static void applyRemoveModes(std::string removeModes, Channel channel, Message msg, int *paramToUse) {
	static_cast<void>(*paramToUse);
	static_cast<void>(msg);
	for (size_t i = 0; i != removeModes.size(); i++) {
		if (removeModes[i] == 'i') {
			channel.setMode(e_i, 0);
		}
		else if (removeModes[i] == 't')
			channel.setMode(e_t, 0);
		else if (removeModes[i] == 'k') {
			channel.setMode(e_k, 0);
			channel.setPassword("");
		}
		else if (removeModes[i] == 'l') {
			channel.setMode(e_l, 0);
			channel.setMaxUsers(-1);
		}
		else if (removeModes[i] == 'o') {
		}
	}
}

//TODO: add the correct RPL is mode solo, for CHANNELMODEIS and CREATIONTIME
void	mode(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);
	size_t msgSize = msg.getParams().size();

	if (msgSize < 1) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return ;
	}

	std::string target = msg.getParams()[0];
	std::map<const std::string, Channel> &channels = serv->getChannels();
	std::map<const std::string, Channel>::iterator itChannel = channels.find(target);
	
	if (itChannel != channels.end()) {
		Channel &channel = itChannel->second;
		std::string modes;
		std::string setModes;
		std::string removeModes;
		int paramToUse = 2;

		if (msgSize == 1) {
			addToClientBuf(serv, clientFd, RPL_CHANNELMODEIS(client.getNickname(), channel.getName()));
			addToClientBuf(serv, clientFd, RPL_CREATIONTIME(client.getNickname(), channel.getName()));
		}
		else if (msgSize >= 2) {
			modes = msg.getParams()[1];
			if (modes[0] == '+') {
				setModes = getSetModes(modes, serv, clientFd, channel.getName());
				removeModes = getRemoveModes(modes, serv, clientFd, channel.getName());
				applySetModes(setModes, channel, msg, &paramToUse, serv, clientFd, client.getNickname());
				applyRemoveModes(removeModes, channel, msg, &paramToUse);
			}
			else if (modes[0] == '-') {
				removeModes = getRemoveModes(modes, serv, clientFd, channel.getName());
				if (removeModes.empty()) {
					addToClientBuf(serv, clientFd, ERR_WRONGMODEFORMAT(channel.getName(), modes));
					return ;
				}
				setModes = getSetModes(modes, serv, clientFd, channel.getName());
				applyRemoveModes(removeModes, channel, msg, &paramToUse);
				applySetModes(setModes, channel, msg, &paramToUse, serv, clientFd, client.getNickname());
			}
			else {
				addToClientBuf(serv, clientFd, ERR_WRONGMODEFORMAT(channel.getName(), modes));
				return ;
			}
		}
	}
	else
		addToClientBuf(serv, clientFd, ERR_NOSUCHCHANNEL(client.getNickname(), msg.getParams()[0]));
}
