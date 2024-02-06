#include "classes/Client.hpp"

Client::Client(const int socket) : _socket(socket), _goodPass(false) {}

Client::Client(const Client &other) : _socket(-1){
	*this = other;
}

Client::~Client() {}

Client &Client::operator=(const Client &other) {

	(void) other;
	return (*this);
}

std::string &Client::getReadBuff() { return (_readBuff); }

std::string &Client::getSendBuff() { return (_sendBuff); }

std::string &Client::getNickname() { return (_nickname); }

std::string &Client::getUsername() { return (_username); }

std::string &Client::getRealName() { return (_realName); }

bool Client::getGoodPass() { return (_goodPass); }

bool Client::getIsRegistered() { return (_isRegistered); }

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