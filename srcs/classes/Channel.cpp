/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:53:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/02/07 12:53:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "classes/Channel.hpp"

Channel::Channel(std::string &name, Server *serv, const int clientFd) : _name(name),
																		_modes(),
																		_maxUsers(-1) {

	Client &client = findClient(serv, clientFd);

	_password.clear();
	_invites.clear();
	_chanOps.insert(std::pair<const int, Client &>(clientFd, client));

	for (int i = 0; i < 3; i++)
		_modes[i] = false;
}

Channel::Channel(const Channel &other) : _modes(), _maxUsers(other._maxUsers) {

	for (int i = 0; i < 3; i++)
		_modes[i] = other._modes[i];
	*this = other;
}

Channel::~Channel() {}

Channel &Channel::operator=(const Channel &other) {

	if (this != &other)
	{
		_members = other._members;
		_chanOps = other._chanOps;
		_name = other._name;
		_password = other._password;
		_maxUsers = other._maxUsers;
		for (int i = 0; i < 3; i++)
			_modes[i] = other._modes[i];
	}
	return (*this);
}

std::string Channel::getName() const { return (_name); }

std::string &Channel::getTopic() { return (_topic); }

std::string Channel::getPassword() const { return (_password); }

std::vector<std::string> Channel::getInvites() const { return (_invites); }

std::vector<Client *> &Channel::getMembers() { return (_members); }

std::map<const int, Client &> &Channel::getChanOps() { return (_chanOps); }

const bool* Channel::getModes() const { return (_modes); };

int Channel::getMaxUsers() const { return (_maxUsers); };

void Channel::setTopic(std::string &newTopic) { _topic = newTopic; }

void Channel::setPassword(std::string newPassword) { _password = newPassword; }

void Channel::setMode(int i, int sign) {
	if (sign == 0)
		_modes[i] = false;
	else if (sign == 1)
		_modes[i] = true;
}

void Channel::setMaxUsers(int newMaxUsers) { _maxUsers = newMaxUsers; }

void Channel::addInvite(std::string &nick) {

	_invites.push_back(nick);
}

void Channel::addMember(Client *newMember) {

	_members.push_back(newMember);
}

void Channel::addChanOps(const int clientFd, Client &newChanOps) {

	_chanOps.insert(std::pair<const int, Client &>(clientFd, newChanOps));
}

/*std::map<const std::string, Channel>::iterator findChannel(Server *serv, std::string const &name)
{
	std::map<const std::string, Channel> channelsList = serv->getChannels();
	std::map<const std::string, Channel>::iterator it = channelsList.find(name);


	if (it == channelsList.end())
		return (NULL);

	return (it);
}*/