/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:20:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/02/16 15:20:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "main.hpp"

static bool	clientOnChannel(int clientFd, std::string &chanName, Server *serv);
static bool invitedOnChannel(int clientFd, std::string &chanName, Server *serv, const std::string &invitedClient);

void invite(Server *serv, Message msg, int clientFd) {

	Client	&client = findClient(serv, clientFd);

	if (msg.getParams().size() < 2) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return ;
	}

	if (!checkClient(serv, msg, clientFd) || !checkChannel(serv, msg, clientFd))
		return ;

	if (!clientOnChannel(clientFd, msg.getParams()[1], serv) || invitedOnChannel(clientFd, msg.getParams()[1], serv, msg.getParams()[0]))
		return ;

	Channel &channel = findChannel(serv, msg.getParams()[1]);
	Client &invitedClient = getClient(serv, msg.getParams()[0]);

	channel.addInvite(msg.getParams()[0]);
	addToClientBuf(serv, clientFd, RPL_INVITING(client.getNickname(), msg.getParams()[0], msg.getParams()[1]));
	addToClientBuf(serv, invitedClient.getSocket(), INVITE(client.getNickname(), msg.getParams()[1]));
}

static bool	clientOnChannel(int	clientFd, std::string &chanName, Server *serv) {
	Client	&client = findClient(serv, clientFd);
	Channel	&channel = findChannel(serv, chanName);

	if (isOperator(client, channel))
		return (true);

	if (channel.getMembers().empty())
		return (false);

	for (size_t i = 0; i < channel.getMembers().size(); i++) {
		if (channel.getMembers()[i]->getNickname() == client.getNickname()) {
			if (!channel.getModes()[e_i])
				return (true);
			else {
				addToClientBuf(serv, client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel.getName()));
				return (false);
			}
		}
	}

	addToClientBuf(serv, client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
	return (false);
}

static bool invitedOnChannel(int clientFd, std::string &chanName, Server *serv, const std::string &invitedClient) {
	Client	&client = findClient(serv, clientFd);
	Client	&clientInvited = getClient(serv, invitedClient);
	Channel &channel = findChannel(serv, chanName);

	if (isOperator(clientInvited, channel))
	{
		addToClientBuf(serv, client.getSocket(), ERR_USERONCHANNEL(client.getNickname(), invitedClient, channel.getName()));
		return (true);
	}

	if (channel.getMembers().empty())
		return (false);

	for (size_t i = 0; i < channel.getMembers().size(); i++) {
		if (channel.getMembers()[i]->getNickname() == invitedClient) {
			addToClientBuf(serv, client.getSocket(), ERR_USERONCHANNEL(client.getNickname(), invitedClient, channel.getName()));
			return (true);
		}
	}

	return (false);
}