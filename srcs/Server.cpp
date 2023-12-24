#include "Server.hpp"

Server::Server(char *port, const std::string &password) : _port(port), _socket(-1), _password(password) {

	memset(_servInfo, 0, sizeof *_servInfo);
}

Server::Server(const Server &other) {
	*this = other;
}

Server::~Server(void) {}

Server &Server::operator=(const Server &other) {

	if (this != &other)
		_port = other._port;
	return (*this);
}

int Server::getServInfo() {

	struct addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status = getaddrinfo(nullptr, _port, &hints, &_servInfo);
	if (status != 0)
	{
		std::cout << ERR_GETADDINFO << gai_strerror(status) << std::endl;
		exit (1);
	}

	_socket = socket(_servInfo->ai_family, _servInfo->ai_socktype, _servInfo->ai_protocol);
	if (_socket < 0)
	{
		std::perror(ERR_SOCKET);
		freeaddrinfo(_servInfo);
		exit (1);
	}

	return (0);
}

void Server::start() const {

	errno = 0;

	bind(_socket, _servInfo->ai_addr, _servInfo->ai_addrlen);
	listen(_socket, 10);
	if (errno != 0)
	{
		std::perror(ERR_START_SERV);
		freeaddrinfo(_servInfo);
		exit (1);
	}
}