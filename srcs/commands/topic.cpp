#include "main.hpp"

//je check si channel topic en chan ops only, si oui je check si chan ops.
//if change topic, send to everyone the topic command

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
				for (std::vector<Client *>::iterator itMember = members.begin(); itMember != members.end(); ++itMember)
					addToClientBuf(serv, (*itMember)->getSocket(), RPL_TOPIC((*itMember)->getNickname(), channel.getName(), channel.getTopic()));
			}
		}
	}
	else
		addToClientBuf(serv, clientFd, ERR_NOSUCHCHANNEL(client.getNickname(), msg.getParams()[0]));
}
