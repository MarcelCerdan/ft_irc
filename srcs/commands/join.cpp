#include "main.hpp"

static void	printChannelInf(Server *serv, int clientFd, Channel &channel);
static bool	existingChan(Channel &channel, Server *serv, int clientFd);
static bool	inviteMode(Channel &channel, std::string &nick);
static bool	checkKey(Server *serv, Message msg, std::string &chanName, size_t i, int clientFd);
static bool	checkChan(Server *serv, int clientFd, std::string chanName);
static void	sendJoinMsg(Server *serv, Channel &channel, int clientFd);

void join(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);

	if (msg.getParams().empty()) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return ;
	}

	std::vector<std::string> targetChannels = splitTargets(msg.getParams()[0]);
	std::map<const std::string, Channel> &channelsList = serv->getChannels();

	for (size_t i = 0; i < targetChannels.size(); i++) {
		if (checkChan(serv, clientFd, targetChannels[i]) && checkKey(serv, msg, targetChannels[i], i, clientFd)) {
			std::map<const std::string, Channel>::iterator it = channelsList.find(targetChannels[i]);

			if (it == channelsList.end())
			{
				Channel newChannel(targetChannels[i], serv, clientFd);
				serv->addChannel(newChannel);
				channelsList = serv->getChannels();
				it = channelsList.find(targetChannels[i]);
				client.addChannel(targetChannels[i]);
				printChannelInf(serv, clientFd, it->second);
			}
			else if (existingChan(it->second, serv, clientFd)) {
				client.addChannel(targetChannels[i]);
				printChannelInf(serv, clientFd, it->second);
			}
		}
	}
}


static bool	checkChan(Server *serv, const int clientFd, std::string chanName)
{

	if (chanName[0] != '#' || chanName.find_first_of(" ,") != std::string::npos) {
		addToClientBuf(serv, clientFd, ERR_BADCHANMASK(chanName));
		return (false);
	}

	return (true);

}

static bool	checkKey(Server *serv, Message msg, std::string &chanName, size_t i, const int clientFd) {
	Channel &chan = findChannel(serv, chanName);
	Client &client = findClient(serv, clientFd);

	if (!chan.getModes()[e_k])
		return (true);

	if (msg.getParams().size() == 1) {
		addToClientBuf(serv, clientFd, WRONGKEY(chanName, client.getNickname()));
		return (false);
	}

	std::vector<std::string> keys = splitTargets(msg.getParams()[1]);

	if (msg.getParams().size() == 1 || keys[i] != chan.getPassword())
	{
		addToClientBuf(serv, clientFd, WRONGKEY(chanName, client.getNickname()));
		return (false);
	}

	return (true);
}

static bool	existingChan(Channel &channel, Server *serv, int clientFd)
{
	Client	*client = &findClient(serv, clientFd);

	std::map<const int, Client &>::iterator itChanOps = channel.getChanOps().find(clientFd);

	for (size_t i = 0; i < channel.getMembers().size(); i++)
	{
		if (channel.getMembers()[i]->getNickname() == client->getNickname())
			return (true);
	}

	if (itChanOps != channel.getChanOps().end())
		return (true);

	if (channel.getModes()[e_l]) {
		size_t clientNb = channel.getChanOps().size() + channel.getMembers().size();
		if (clientNb >= (size_t) channel.getMaxUsers()) {
			addToClientBuf(serv, clientFd, ERR_CHANNELISFULL(client->getNickname(), channel.getName()));
			return (false);
		}
	}

	if (inviteMode(channel, client->getNickname()))
		channel.addMember(client);
	else
	{
		addToClientBuf(serv, clientFd, ERR_INVITEONLYCHAN(client->getNickname(), channel.getName()));
		return (false);
	}

	return (true);
}

static bool	inviteMode(Channel &channel, std::string &nick)
{
	if (!channel.getModes()[e_i])
		return (true);

	std::vector<std::string> invites = channel.getInvites();
	for (size_t i = 0; i < invites.size(); i++)
	{
		if (invites[i] == nick)
			return (true);
	}

	return (false);
}

static void	printChannelInf(Server *serv, int clientFd, Channel &channel)
{
	Client *client = &findClient(serv, clientFd);

	sendJoinMsg(serv, channel, clientFd);
	if (channel.getTopic().empty())
		addToClientBuf(serv, clientFd, RPL_NOTOPIC(client->getNickname(), channel.getName()));
	else
		addToClientBuf(serv, clientFd, RPL_TOPIC(client->getNickname(), channel.getName(), channel.getTopic()));

	addToClientBuf(serv, clientFd, RPL_NAMREPLY(client->getNickname(), "=", channel.getName()));
	std::map<const int, Client &>::iterator it_chanOps = channel.getChanOps().begin();
	std::size_t i = 0;
	while ( i != channel.getChanOps().size())
	{
		if (it_chanOps->first > 2)
			addToClientBuf(serv, clientFd, "@" + it_chanOps->second.getNickname() + " ");
		it_chanOps++;
		i++;
	}
	if (!channel.getMembers().empty())
	{
		i = 0;
		while (i < channel.getMembers().size())
		{
			addToClientBuf(serv, clientFd, channel.getMembers()[i]->getNickname() + " ");
			i++;
		}
	}
	addToClientBuf(serv, clientFd, "\r\n");
	addToClientBuf(serv, clientFd, RPL_ENDOFNAMES(client->getNickname(), channel.getName()));
}

static void	sendJoinMsg(Server *serv, Channel &channel, int clientFd) {
	Client &client = findClient(serv, clientFd);
	std::map<const int, Client &> &chanOps = channel.getChanOps();

	for (std::map<const int, Client &>::iterator it = chanOps.begin(); it != chanOps.end(); it++)
		addToClientBuf(serv, it->second.getSocket(), JOINCHANNEL(client.getNickname(), channel.getName()));

	for (std::vector<Client *>::iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); it++)
		addToClientBuf(serv, (*it)->getSocket(), JOINCHANNEL(client.getNickname(), channel.getName()));
}