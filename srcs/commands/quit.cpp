#include "main.hpp"

static void	findClientChannels(Server *serv, int clientFd, std::string &quitMsg);
static void	sendQuitMsg (Server *serv, std::string &chanName, std::string &quitMsg, int clientFd);

void quit(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);
	std::string reason;

	client.setIsConnected(false);
	if (!msg.getParams().empty())
		reason = msg.getParams()[0];

	findClientChannels(serv, clientFd, reason);
	addToClientBuf(serv, clientFd, ":localhost ERROR :Closing connection\r\n");
}

static void	findClientChannels(Server *serv, int clientFd, std::string &quitMsg) {
	Client &client = findClient(serv, clientFd);
	std::vector<std::string> &clientChannels = client.getChannels();

	for (std::vector<std::string>::iterator it = clientChannels.begin(); it != clientChannels.end(); it++)
		sendQuitMsg(serv, *it, quitMsg, clientFd);
}

static void	sendQuitMsg (Server *serv, std::string &chanName, std::string &quitMsg, int clientFd) {
	Channel &chan = findChannel(serv, chanName);
	Client &client = findClient(serv, clientFd);

	for (std::map<const int, Client &>::iterator it = chan.getChanOps().begin(); it != chan.getChanOps().end(); it++) {
		if (it->second.getSocket() != clientFd)
			addToClientBuf(serv, it->second.getSocket(), QUITMSG(client.getNickname(), quitMsg));
	}

	for (std::vector<Client *>::iterator it = chan.getMembers().begin(); it != chan.getMembers().end(); it++) {
		if ((*it)->getSocket() != clientFd)
			addToClientBuf(serv, (*it)->getSocket(), QUITMSG(client.getNickname(), quitMsg));
	}
}
