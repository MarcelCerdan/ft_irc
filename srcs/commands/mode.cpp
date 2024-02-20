#include "main.hpp"
#include <sstream>

static bool isNumeric(const std::string& str) {
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

static void applySetModes(std::string setModes, Channel &channel, Message msg, int *paramToUse, Server *serv, int clientFd, std::string nick) {
	for (size_t i = 0; i != setModes.size(); i++) {
		if (setModes[i] == 'i')
			channel.setMode(e_i, 1);
		else if (setModes[i] == 't')
			channel.setMode(e_t, 1);
		else if (setModes[i] == 'k') {
			if (static_cast<size_t>(*paramToUse) < msg.getParams().size()) {
				std::string keyword = msg.getParams()[*paramToUse];

				if (keyword.find(' ') != std::string::npos) {
					addToClientBuf(serv, clientFd, ":localhost " + nick + " :keyword can't contain space character (" ", 0x20)");
					return ;
				}
				channel.setMode(e_k, 1);
				channel.setPassword(keyword);
				*paramToUse += 1;
			}
			else
				addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(nick, "MODE"));
		}
		else if (setModes[i] == 'l') {
			if (static_cast<size_t>(*paramToUse) < msg.getParams().size()) {
				if (isNumeric(msg.getParams()[*paramToUse])) {
					int newMaxUser = atoi(msg.getParams()[*paramToUse].c_str());
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
			std::string memberToChange = msg.getParams()[*paramToUse];
			std::vector<Client *> &members = channel.getMembers();
			bool memberFound = false;

			for (std::vector<Client *>::iterator itMember = members.begin(); itMember != members.end(); itMember++) {
				if ((*itMember)->getNickname() == memberToChange) {
					channel.addChanOps((*itMember)->getSocket(), **itMember);
					members.erase(itMember);
					*paramToUse += 1;
					memberFound = true;
					break ;
				}
			}
			if (!memberFound)
				addToClientBuf(serv, clientFd, ERR_NOSUCHNICK(memberToChange));
		}
		else
			addToClientBuf(serv, clientFd, ERR_MODEUNKNOWFLAG(channel.getName(), setModes[i]));
	}
}

static void applyRemoveModes(std::string removeModes, Channel &channel, Message msg, int *paramToUse, Server *serv, int clientFd) {
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
			std::string chanOpsToChange = msg.getParams()[*paramToUse];
			std::map<const int, Client &> &chanOps = channel.getChanOps();
			bool chanOpsFound = false;

			for (std::map<const int, Client &>::iterator itChanOps = chanOps.begin(); itChanOps != chanOps.end(); itChanOps++) {
				if (itChanOps->second.getNickname() == chanOpsToChange) {
					channel.addMember(&(itChanOps->second));
					chanOps.erase(itChanOps);
					*paramToUse += 1;
					chanOpsFound = true;
					break ;
				}
			}
			if (!chanOpsFound)
				addToClientBuf(serv, clientFd, ERR_NOSUCHNICK(chanOpsToChange));
		}
		else
			addToClientBuf(serv, clientFd, ERR_MODEUNKNOWFLAG(channel.getName(), removeModes[i]));
	}
}

static void displayAllModes(Server *serv, Client &client, Channel &channel, int clientFd) {
	addToClientBuf(serv, clientFd, RPL_CHANNELMODEIS(client.getNickname(), channel.getName()));
	addToClientBuf(serv, clientFd, "\tInvite-only : ");
	if (channel.getModes()[e_i])
		addToClientBuf(serv, clientFd, "yes");
	else
		addToClientBuf(serv, clientFd, "no");
	addToClientBuf(serv, clientFd, "\n");
	addToClientBuf(serv, clientFd, "\tCan change Topic: ");
	if (channel.getModes()[e_t])
		addToClientBuf(serv, clientFd, "Channel Operators");
	else
		addToClientBuf(serv, clientFd, "everyone");
	addToClientBuf(serv, clientFd, "\n");
	addToClientBuf(serv, clientFd, "\tPassword required: ");
	if (channel.getModes()[e_k]) {
		addToClientBuf(serv, clientFd, "yes");
		if (isOperator(client, channel))
			addToClientBuf(serv, clientFd, " password: " + channel.getPassword());
	}
	else
		addToClientBuf(serv, clientFd, "no");
	addToClientBuf(serv, clientFd, "\n");
	addToClientBuf(serv, clientFd, "\tUser limit to channel: ");
	if (channel.getModes()[e_l])
		addToClientBuf(serv, clientFd, intToString(channel.getMaxUsers()));
	else
		addToClientBuf(serv, clientFd, "none");
	addToClientBuf(serv, clientFd, "\r\n");

	std::string creationDate = channel.getCreationDate();

	addToClientBuf(serv, clientFd, RPL_CREATIONTIME(client.getNickname(), channel.getName(), creationDate));
}

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

		if (msgSize == 1)
			displayAllModes(serv, client, channel, clientFd);
		else if (msgSize >= 2 && isOperator(client, channel)) {
			modes = msg.getParams()[1];
			if (modes[0] == '+') {
				setModes = getSetModes(modes, serv, clientFd, channel.getName());
				removeModes = getRemoveModes(modes, serv, clientFd, channel.getName());
				applySetModes(setModes, channel, msg, &paramToUse, serv, clientFd, client.getNickname());
				applyRemoveModes(removeModes, channel, msg, &paramToUse, serv, clientFd);
			}
			else if (modes[0] == '-') {
				removeModes = getRemoveModes(modes, serv, clientFd, channel.getName());
				if (removeModes.empty()) {
					addToClientBuf(serv, clientFd, ERR_WRONGMODEFORMAT(channel.getName(), modes));
					return ;
				}
				setModes = getSetModes(modes, serv, clientFd, channel.getName());
				applyRemoveModes(removeModes, channel, msg, &paramToUse, serv, clientFd);
				applySetModes(setModes, channel, msg, &paramToUse, serv, clientFd, client.getNickname());
			}
			else {
				addToClientBuf(serv, clientFd, ERR_WRONGMODEFORMAT(channel.getName(), modes));
				return ;
			}
		}
		else {
			addToClientBuf(serv, clientFd, ERR_CHANOPRIVSNEEDED(client.getNickname(), channel.getName()));
		}
	}
	else
		addToClientBuf(serv, clientFd, ERR_NOSUCHCHANNEL(client.getNickname(), msg.getParams()[0]));
}
