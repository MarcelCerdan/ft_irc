#include "Client.hpp"

Client::Client(const int socket) : _socket(socket) {}

Client::Client(const Client &other) : _socket(-1){
	*this = other;
}

Client::~Client() {}

Client &Client::operator=(const Client &other) {
	//if (this != &other)
	//	_socket = other._socket;
	(void) other;
	return (*this);
}