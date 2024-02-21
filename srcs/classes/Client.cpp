#include "classes/Client.hpp"

Client::Client(const int socket) : _socket(socket), _goodPass(false), _isRegistered(false) {}

Client::Client(const Client &other) : _socket(other._socket), _goodPass(other._goodPass), _isRegistered(other._isRegistered) {
	*this = other;
}

Client::~Client() {}

Client &Client::operator=(const Client &other) {

	if (this != &other)
	{
		_readBuff = other._readBuff;
		_sendBuff = other._sendBuff;
		_nickname = other._nickname;
		_username = other._username;
		_realName = other._realName;
		_goodPass = other._goodPass;
		_isRegistered = other._isRegistered;
	}
	return (*this);
}

std::string &Client::getReadBuff() { return (_readBuff); }

std::string &Client::getSendBuff() { return (_sendBuff); }

std::string &Client::getNickname() { return (_nickname); }

std::string &Client::getUsername() { return (_username); }

std::string &Client::getRealName() { return (_realName); }

std::vector<std::string> &Client::getChannels() { return (_channels); }

bool &Client::getGoodPass() { return (_goodPass); }

bool &Client::getIsRegistered() { return (_isRegistered); }

int Client::getSocket() { return (_socket); }

void Client::setReadBuff(const std::string &msg) {

	_readBuff += msg;
}

void Client::setSendBuff(const std::string &msg) {

	_sendBuff += msg;
}

void Client::setNickname(const std::string &newNickname) { _nickname = newNickname; }

void Client::setGoodPass() { _goodPass = true; }

void Client::setRealName(const std::string &realName) { _realName = realName; }

void Client::setUsername(const std::string &username) { _username = username; }

void Client::setIsRegister() { _isRegistered = true; }

void Client::freeSendBuffer() { _sendBuff.clear(); }

void Client::freeReadBuffer() { _readBuff.clear(); }

void Client::addChannel(const std::string &channel) {
	_channels.push_back(channel);
}

void Client::removeChannel(std::string const &channel) {
	for (std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		if (*it == channel) {
			_channels.erase(it);
			break ;
		}
	}
}