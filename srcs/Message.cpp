/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthibaul <mthibaul@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:14:00 by mthibaul          #+#    #+#             */
/*   Updated: 2024/01/27 00:14:00 by mthibaul         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "../headers/Message.hpp"

Message::Message(std::string msg) : _fullMsg(msg) {}

Message::Message(const Message &other) {

	*this = other;
}

Message::~Message(void) {}

Message &Message::operator=(const Message &other) {

	if (this != &other) {
		_cmd = other._cmd;
		_param = other._param;
		_prefix = other._prefix;
	}
	return (*this);
}

std::string const &Message::getFullMsg() { return (_fullMsg); }

std::vector<std::string> &Message::getSplitMsg() { return (_splitMsg); }

std::string &Message::getPrefix() { return (_prefix); }

std::string &Message::getCmd() { return (_cmd); }

std::string &Message::getParam() { return (_param); }

void Message::splitMsg(std::string const &delimiter) {

	std::size_t	pos = _fullMsg.find(delimiter);
	std::string	substr;

	while (pos != std::string::npos)
	{
		substr = _fullMsg.substr(0, pos);
		_splitMsg.push_back(substr);
		pos = _fullMsg.find(delimiter, pos);
	}

}
