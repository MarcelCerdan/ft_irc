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
		std::cout << RED << ERR_GETADDINFO << gai_strerror(status) << RESET << std::endl;
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

	std::cout << GREEN << "Server #" << _socket << " listening on port " << _port << RESET << std::endl;

	launchServLoop();
}

void Server::launchServLoop() {

	std::vector<pollfd>	pfds;
	pollfd				server_pfd;

	server_pfd.fd = _socket;
	server_pfd.events = POLLIN;
	pfds.push_back(server_pfd);

	while (1)
	{
		std::vector<pollfd> newPfds;

		int events = poll((pollfd *)&pfds[0], pfds.size(), -1);
		if (events < 0)
		{
			std::cerr << ERR_POLL << std::endl;
			exit (1);
		}

		int eventsFound = 0;
		std::vector<pollfd>::iterator it = pfds.begin();
		while (it != pfds.end() && eventsFound < events)
		{
			if (it->revents & POLLIN)
			{
				eventsFound++;
				if (it->fd == _socket)
					newClient(pfds, newPfds);
				else
				{
					return;
					//handle existing connection
				}
			}
			it++;
		}

	}
}

int Server::newClient(std::vector<pollfd> pfds, std::vector<pollfd> newPfds) {

	sockaddr_in client;
	socklen_t addr_size = sizeof(sockaddr_in);

	int clientSocket = accept(_socket, (sockaddr *)&client, &addr_size);
	if (clientSocket < 0)
	{
		std::cerr << ERR_ACCEPT << std::endl;
		return (1);
	}
	if (pfds.size() - 1 < MAX_CLIENT)
	{
		pollfd	clientPfd;
		Client	newClient(clientSocket);

		clientPfd.fd = clientSocket;
		clientPfd.events = POLLIN | POLLOUT;
		newPfds.push_back(clientPfd);
		_clients.insert(std::pair<const int, Client>(clientSocket, newClient));

		std::cout << GREEN << "Server added client #" << clientSocket << RESET << std::endl;
	}
	else
	{
		std::cout << RED << ERR_FULL_SERV << RESET << std::endl;
		send(clientSocket, ERR_FULL_SERV, strlen(ERR_FULL_SERV) + 1, MSG_NOSIGNAL);
		close(clientSocket);
	}
	return (0);
}