#include "main.hpp"

static bool	checkErrors(Server *serv, std::string channelName, const int clientFd);
static void	sendPartMsg(Server *serv, int clientFd, std::string channelName, std::string msg);

void	part(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);

	if (msg.getParams().empty()) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return ;
	}

	std::vector<std::string> splitedChannels = splitTargets(msg.getParams()[0]);

	std::string partMsg;
	if (msg.getParams().size() > 1)
		partMsg = msg.getParams()[1];

	for (size_t i = 0; i < splitedChannels.size(); i++) {
		if (checkErrors(serv, splitedChannels[i], clientFd)) {
			Channel &channel = findChannel(serv, splitedChannels[i]);

			sendPartMsg(serv, clientFd, splitedChannels[i], partMsg);
			channel.eraseMember(serv, clientFd);
			serv->removeChannel(splitedChannels[i]);
		}	
	}
}

static bool	checkErrors(Server *serv, std::string channelName, const int clientFd) {
	Client &client = findClient(serv, clientFd);

	if (!checkChannel(serv, channelName, clientFd))
		return (false);

	Channel &channel = findChannel(serv, channelName);

	if (!isMember(client, channel) && !isOperator(client, channel)) {
		addToClientBuf(serv, clientFd, ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
		return (false);
	}

	return (true);
}

static void	sendPartMsg(Server *serv, int clientFd, std::string channelName, std::string msg) {
	Client &client = findClient(serv, clientFd);
	Channel &channel = findChannel(serv, channelName);

	if (!msg.empty())
		msg = " :" + msg;

	for (std::map<const int, Client &>::iterator it = channel.getChanOps().begin(); it != channel.getChanOps().end(); it++)
		addToClientBuf(serv, it->first, PARTMSG(client.getNickname(), channelName, msg));

	for (size_t i = 0; i < channel.getMembers().size(); i++)
		addToClientBuf(serv, channel.getMembers()[i]->getSocket(), PARTMSG(client.getNickname(), channelName, msg));
}