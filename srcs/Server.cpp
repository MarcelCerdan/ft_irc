#include "Server.hpp"

Server::Server(char *port, const std::string &password) : _port(port), _socket(-1), _password(password) {}

Server::Server(const Server &other) {
	*this = other;
}

Server::~Server(void) {}

Server &Server::operator=(const Server &other) {

	if (this != &other)
		_port = other._port;
	return (*this);
}

void Server::start() {

	errno = 0;

	addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status = getaddrinfo(NULL, _port, &hints, &_servInfo);
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

	bind(_socket, _servInfo->ai_addr, _servInfo->ai_addrlen);
	listen(_socket, 10);
	freeaddrinfo(_servInfo);
	if (errno != 0)
	{
		std::perror(ERR_START_SERV);
		exit (1);
	}

	std::cout << "Serveur listening on port : " << _port << std::endl;

	launchServLoop();

	/*const char *message = "Bonjour !";

	ssize_t byteSent = send(new_fd, message, std::strlen(message), MSG_NOSIGNAL);

	if (byteSent < 0)
		std::cout << "Error with send" << std::endl;
	else
		std::cout << "Sent " << byteSent << " bytes" << std::endl;*/
}

void Server::launchServLoop() const {

	sockaddr_storage client_addr;
	socklen_t addr_size;

	poll(*_pfds, 10, 2500);
	while (1)
	{
		addr_size = sizeof client_addr;
		int	new_fd = accept(_socket, (struct sockaddr *)&client_addr, &addr_size);
		if (errno != 0)
		{
			std::perror(ERR_START_SERV);
			exit (1);
		}

		(void) new_fd;
	}
}