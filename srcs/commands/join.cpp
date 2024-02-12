/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 13:31:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/02/07 13:31:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "main.hpp"

void join(Server *serv, Message msg, int clientFd)
{
	std::map<const std::string, Channel> &channelsList = serv->getChannels();
	std::map<const std::string, Channel>::iterator it = channelsList.find(msg.getParams()[0]);

	Client *client = &findClient(serv, clientFd);
	if (msg.getParams().empty())
	{
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client->getNickname(), msg.getCmd()));
		return;
	}

	if (it == channelsList.end())
	{
		Channel newChannel(msg.getParams()[0], serv, clientFd);
		serv->addChannel(newChannel);
		channelsList = serv->getChannels();
		it = channelsList.find(msg.getParams()[0]);
	}
	else
		it->second.addMember(client);

	Channel &channel = it->second;
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