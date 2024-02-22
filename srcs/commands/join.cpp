#include "main.hpp"

void	printChannelInf(Server *serv, int clientFd, Channel &channel);
bool	existingChan(Channel &channel, Server *serv, int clientFd);
bool	inviteMode(Channel &channel, std::string &nick);
bool	checkChanName(std::string chanName);
bool	checkJoinParams(Message &msg, Server *serv, int clientFd);


/* TO DO : Handle keys in channels and list of channels to join */

void join(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);

	if (!checkJoinParams(msg, serv, clientFd))
		return ;

	std::map<const std::string, Channel> &channelsList = serv->getChannels();
	std::map<const std::string, Channel>::iterator it = channelsList.find(msg.getParams()[0]);


	if (it == channelsList.end())
	{
		Channel newChannel(msg.getParams()[0], serv, clientFd);
		serv->addChannel(newChannel);
		channelsList = serv->getChannels();
		it = channelsList.find(msg.getParams()[0]);
	}
	else if (!existingChan(it->second, serv, clientFd))
		return;

	client.addChannel(msg.getParams()[0]);
	printChannelInf(serv, clientFd, it->second);
}

bool	checkJoinParams(Message &msg, Server *serv, int clientFd)
{
	Client &client = findClient(serv, clientFd);

	if (msg.getParams().empty())
	{
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return (false);
	}

	if (!checkChanName(msg.getParams()[0]))
	{
		addToClientBuf(serv, clientFd, ERR_BADCHANMASK(msg.getParams()[0]));
		return (false);
	}

	return (true);
}

bool	checkChanName(std::string chanName)
{
	if (chanName[0] != '#')
		return (false);

	if (chanName.find_first_of(" ,") != std::string::npos)
		return (false);
	return (true);

}

bool	existingChan(Channel &channel, Server *serv, int clientFd)
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

	if (inviteMode(channel, client->getNickname()))
		channel.addMember(client);
	else
	{
		addToClientBuf(serv, clientFd, ERR_INVITEONLYCHAN(client->getNickname(), channel.getName()));
		return (false);
	}

	return (true);
}

bool	inviteMode(Channel &channel, std::string &nick)
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

void	printChannelInf(Server *serv, int clientFd, Channel &channel)
{
	Client *client = &findClient(serv, clientFd);

	addToClientBuf(serv, clientFd, JOINCHANNEL(client->getNickname(), channel.getName()));
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