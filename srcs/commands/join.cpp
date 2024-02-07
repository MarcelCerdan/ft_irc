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
	Client *client = findClient(serv, clientFd);
	Channel *channel = findChannel(serv, msg.getParams()[0]);

	if (msg.getParams().empty())
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client->getNickname(), msg.getCmd()));

	else if (!channel)
	{
		Channel newChannel(msg.getParams()[0], serv, clientFd);
		serv->addChannel(msg.getParams()[0], newChannel);
		channel = &newChannel;
	}
	else
	{
		channel->addMember(client);
	}

	addToClientBuf(serv, clientFd, JOINCHANNEL(client->getNickname(), channel->getName()));
	if (channel->getTopic().empty())
		addToClientBuf(serv, clientFd, RPL_NOTOPIC(client->getNickname(), channel->getName()));
	else
		addToClientBuf(serv, clientFd, RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()));

	addToClientBuf(serv, clientFd, RPL_NAMREPLY(client->getNickname(), "=", channel->getName()));
	std::map<const int, Client *>::iterator it = channel->getChanOps().begin();
	std::size_t i = 0;
	while ( i != channel->getChanOps().size())
	{
		if (it->first > 2)
			addToClientBuf(serv, clientFd, "@" + it->second->getNickname() + " ");
		it++;
		i++;
	}
	if (!channel->getMembers().empty())
	{
		i = 0;
		while (i < channel->getMembers().size())
		{
			addToClientBuf(serv, clientFd, channel->getMembers()[i]->getNickname() + " ");
			i++;
		}
	}
	addToClientBuf(serv, clientFd, "\r\n");
	addToClientBuf(serv, clientFd, RPL_ENDOFNAMES(client->getNickname(), channel->getName()));
}