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

static bool	clientOnChannel(Client &client, Channel &channel, Server *serv);
static bool invitedOnChannel(Client &client, Channel &channel, Server *serv, const std::string &invitedClient);

void invite(Server *serv, Message msg, int clientFd) {

	Client	&client = findClient(serv, clientFd);

	if (msg.getParams().size() < 2) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return ;
	}

	if (!checkClient(serv, msg, clientFd) || !checkChannel(serv, msg, clientFd))
		return ;

	Channel &channel = findChannel(serv, msg.getParams()[1]);
	if (!clientOnChannel(client, channel, serv) || invitedOnChannel(client, channel, serv, msg.getParams()[0]))
		return ;

	Client &invitedClient = getClient(serv, msg.getParams()[0]);

	channel.addInvite(msg.getParams()[0]);
	addToClientBuf(serv, clientFd, RPL_INVITING(client.getNickname(), msg.getParams()[0], msg.getParams()[1]));
	addToClientBuf(serv, invitedClient.getSocket(), INVITE(client.getNickname(), msg.getParams()[1]));
}

static bool	clientOnChannel(Client &client, Channel &channel, Server *serv) {
	std::map<const int, Client &> chanOps = channel.getChanOps();

	for (std::map<const int, Client &>::iterator it = chanOps.begin(); it != chanOps.end(); it++) {
		std::cout << "Key: " << it->first << std::endl;
		if (it->second.getNickname() == client.getNickname())
			return (true);
	}

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

static bool invitedOnChannel(Client &client, Channel &channel, Server *serv, const std::string &invitedClient) {
	for (std::map<const int, Client &>::iterator it = channel.getChanOps().begin(); it != channel.getChanOps().end(); it++) {
		if (it->second.getNickname() == invitedClient) {
			addToClientBuf(serv, client.getSocket(), ERR_USERONCHANNEL(client.getNickname(), invitedClient, channel.getName()));
			return (true);
		}
	}

	std::cout << "OKI" << std::endl;
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