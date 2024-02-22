#include "main.hpp"
#include <sstream>

void privmsg(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);

	if (msg.getParams().size() < 2) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return ;
	}

	std::string targets = msg.getParams()[0];
	std::vector<std::string> stringChannels;
	std::vector<std::string> stringClients;
	std::string preMessage;
	std::string	message;
	std::map<const int, Client> &clients = serv->getClients();
	std::map<const std::string, Channel> &channels = serv->getChannels();

	preMessage = ":" + client.getNickname() + " PRIVMSG ";
	message = " :" + msg.getParams()[1] + "\r\n";

	std::istringstream iss(targets);
	std::string target;
	while (std::getline(iss, target, ',')) {
		if (!target.empty() && target[0] == '#')
			stringChannels.push_back(target);
		else
			stringClients.push_back(target);
	}

	for (std::vector<std::string>::iterator it1 = stringClients.begin(); it1 != stringClients.end(); it1++) {
		std::string targetClientName = *it1;
		bool correct = false;
		for (std::map<const int, Client>::iterator itClient = clients.begin(); itClient != clients.end(); itClient++) {
			if (targetClientName == itClient->second.getNickname()) {
				addToClientBuf(serv, itClient->second.getSocket(), preMessage + itClient->second.getNickname() + message);
				correct = true;
			}
		}
		if (!correct)
			addToClientBuf(serv, clientFd, ERR_NOSUCHNICK(targetClientName));
	}

	for (std::vector<std::string>::iterator it1 = stringChannels.begin(); it1 != stringChannels.end(); it1++) {
		std::string targetChannelName = *it1;
		std::map<std::string, Channel>::iterator itChannel = channels.find(targetChannelName);			
		if (itChannel != channels.end()) {
			std::vector<Client *> &members = itChannel->second.getMembers();
			for (std::vector<Client *>::iterator itMember = members.begin(); itMember != members.end(); itMember++) {
				if ((*itMember)->getSocket() != clientFd)
					addToClientBuf(serv, (*itMember)->getSocket(), preMessage + targetChannelName + message);
			}

			std::map<const int, Client &> &chanOps = itChannel->second.getChanOps();
			for (std::map<const int, Client &>::iterator itChanOps = chanOps.begin(); itChanOps != chanOps.end(); itChanOps++) {
				if (itChanOps->second.getSocket() != clientFd)
					addToClientBuf(serv, itChanOps->second.getSocket(), preMessage + targetChannelName + message);
			}
		}
		else
			addToClientBuf(serv, clientFd, ERR_CANNOTSENDTOCHAN(client.getNickname(), targetChannelName));
	}
}
