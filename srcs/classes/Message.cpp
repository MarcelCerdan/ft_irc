#include "classes/Message.hpp"

Message::Message(std::string msg) : _fullMsg(msg) {}

Message::Message(const Message &other) {
	*this = other;
}

Message::~Message() {}

Message &Message::operator=(const Message &other) {

	if (this != &other) {
		_cmd = other._cmd;
		_params = other._params;
		_prefix = other._prefix;
	}
	return (*this);
}

std::string const &Message::getFullMsg() { return (_fullMsg); }

std::vector<std::string> &Message::getSplitMsg() { return (_splitMsg); }

std::string &Message::getPrefix() { return (_prefix); }

std::string &Message::getCmd() { return (_cmd); }

std::vector<std::string> &Message::getParams() { return (_params); }

void Message::splitMsg() {
	std::string	substr;
	std::stringstream msg(_fullMsg);

	while (std::getline(msg, substr))
		_splitMsg.push_back(substr);
}

void Message::splitParams(std::string *params) {
	std::string substr;
	std::size_t posSpace;
	std::size_t posColon;

	_params.clear();
	while ((posSpace = params->find(' ')) != std::string::npos)
	{
		posColon = params->find(':');
		if (posColon != std::string::npos && posSpace > posColon) {
			substr = params->substr(posColon + 1);
			_params.push_back(substr);
			return ;
		}
		substr = params->substr(0, posSpace);
		if (!substr.empty())
			_params.push_back(substr);
		params->erase(0, posSpace + 1);
	}

	posColon = params->find(':');
	if (posColon != std::string::npos) {
		substr = params->substr(posColon + 1);
		_params.push_back(substr);
	}
	else {
		if (!params->empty())
			_params.push_back(*params);
	}
}
