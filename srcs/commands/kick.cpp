/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:42:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/05/21 17:20:50 by mthibaul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

std::vector<std::string>	splitTargets(std::string &targets);
static bool	checkErrors(Server *serv, Message msg, int clientFd);
static bool clientIsOperator(int clientFd, std::string &chanName, Server *serv);
static bool targetIsOnChannel(int clientFd, std::string &chanName, Server *serv, const std::string &targetName);
static void	sendKickMsg(Server *serv, int clientFd, std::string &targetNick, Message &msg);

void	kick(Server *serv, Message msg, int clientFd) {

	if (!checkErrors(serv, msg, clientFd))
		return ;

	std::vector<std::string> targets = splitTargets(msg.getParams()[1]);

	for (size_t i = 0; i < targets.size(); i++) {	
		if (checkClient(serv, targets[i], clientFd) && targetIsOnChannel(clientFd, msg.getParams()[0], serv, targets[i])) {
			Client &target = getClient(serv, targets[i]);
			Channel &channel = findChannel(serv, msg.getParams()[0]);

			sendKickMsg(serv, clientFd, targets[i], msg);

			channel.eraseMember(serv, target.getSocket());
		}
	}
}

static bool	checkErrors(Server *serv, Message msg, int clientFd) {

	Client &client = findClient(serv, clientFd);

	if (msg.getParams().size() < 2) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return (false);
	}

	if (!checkChannel(serv, msg.getParams()[0], clientFd))
		return (false);

	if (!clientIsOperator(clientFd, msg.getParams()[0], serv))
		return (false);

	return (true);
}

static bool clientIsOperator(int clientFd, std::string &chanName, Server *serv) {
	Client	&client = findClient(serv, clientFd);
	Channel	&channel = findChannel(serv, chanName);

	if (isOperator(client, channel))
		return (true);

	if (channel.getMembers().empty()) {
		addToClientBuf(serv, client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
		return (false);
	}

	for (size_t i = 0; i < channel.getMembers().size(); i++) {
		if (channel.getMembers()[i]->getNickname() == client.getNickname()) {
			addToClientBuf(serv, client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel.getName()));
			return (false);
		}
	}

	addToClientBuf(serv, client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
	return (false);
}

static bool targetIsOnChannel(int clientFd, std::string &chanName, Server *serv, const std::string &targetName) {
	Client &client = findClient(serv, clientFd);
	Client &target = getClient(serv, targetName);
	Channel &channel = findChannel(serv, chanName);

	if (targetName == client.getNickname()) {
		addToClientBuf(serv, clientFd, ":localhost " + client.getNickname() + " KICK :You can't kick yourself\r\n");
		return (false);
	}

	if (channel.getChanOps().find(target.getSocket()) != channel.getChanOps().end())
		return (true);

	for (std::vector<Client *>::iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); it++) {
		if ((*it)->getNickname() == targetName)
			return (true);
	}

	addToClientBuf(serv, clientFd, ERR_USERNOTINCHANNEL(client.getNickname(), targetName, chanName));
	return (false);
}

static void	sendKickMsg(Server *serv, int clientFd, std::string &targetNick, Message &msg) {
	Client &client = findClient(serv, clientFd);
	Client &target = getClient(serv, targetNick);
	Channel &channel = findChannel(serv, msg.getParams()[0]);
	std::string kickMsg;

	if (msg.getParams().size() > 2)
		kickMsg = ":" + msg.getParams()[2];

	for (std::map<const int, Client &>::iterator it = channel.getChanOps().begin(); it != channel.getChanOps().end(); it++)
		addToClientBuf(serv, it->first, KICKMSG(client.getNickname(), targetNick, msg.getParams()[0], kickMsg));

	for (size_t i = 0; i < channel.getMembers().size(); i++)
		addToClientBuf(serv, channel.getMembers()[i]->getSocket(), KICKMSG(channel.getMembers()[i]->getNickname(), targetNick, msg.getParams()[0], kickMsg));

	if (!kickMsg.empty())
		kickMsg = "for " + kickMsg;
	addToClientBuf(serv, target.getSocket(), KICKTARGETMSG(client.getNickname(), msg.getParams()[0], kickMsg));
}