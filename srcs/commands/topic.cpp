#include "main.hpp"

void topic(Server *serv, Message msg, int clientFd) {
	Client &client = findClient(serv, clientFd);
	size_t paramsSize = msg.getParams().size();

	if (paramsSize < 1) {
		addToClientBuf(serv, clientFd, ERR_NEEDMOREPARAMS(client.getNickname(), msg.getCmd()));
		return ;
	}

	std::string target = msg.getParams()[0];
	std::map<const std::string, Channel> &channels = serv->getChannels();
	std::map<const std::string, Channel>::iterator itChannel = channels.find(target);
	
	if (itChannel != channels.end()) {
		Channel &channel = itChannel->second;

		if (isOperator(client, channel) || isMember(client, channel)) {
			if (channel.getModes()[e_t] && !isOperator(client, channel)) {
				addToClientBuf(serv, clientFd, ERR_CHANOPRIVSNEEDED(client.getNickname(), channel.getName()));
				return ;
			}
			if (paramsSize == 1) {
				if (!channel.getTopic().empty())
					addToClientBuf(serv, clientFd, RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic()));
				else
					addToClientBuf(serv, clientFd, RPL_NOTOPIC(client.getNickname(), channel.getName()));
			}
			else {
				std::string newTopic = msg.getParams()[1];

				if (newTopic != channel.getTopic()) {
					channel.setTopic(newTopic);
					std::vector<Client *> &members = channel.getMembers();
					std::map<const int, Client &> &chanOps = channel.getChanOps();

					for (std::vector<Client *>::iterator itMember = members.begin(); itMember != members.end(); ++itMember)
						addToClientBuf(serv, (*itMember)->getSocket(), RPL_TOPIC((*itMember)->getNickname(), channel.getName(), channel.getTopic()));
					for (std::map<const int, Client &>::iterator itChanOps = chanOps.begin(); itChanOps != chanOps.end(); ++itChanOps)
						addToClientBuf(serv, itChanOps->second.getSocket(), RPL_TOPIC(itChanOps->second.getNickname(), channel.getName(), channel.getTopic()));
				}
			}
		}
		else
			addToClientBuf(serv, clientFd, ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
	}
	else
		addToClientBuf(serv, clientFd, ERR_NOSUCHCHANNEL(client.getNickname(), msg.getParams()[0]));
}
