
#include "classes/Server.hpp"

Server::Server(char *port, const std::string &password) : _port(port), _socket(-1), _password(password) {

	_cmdList.insert(std::pair<const std::string, cmdFunction>("PASS", &pass));
	_cmdList.insert(std::pair<const std::string, cmdFunction>("NICK", &nick));
	_cmdList.insert(std::pair<const std::string, cmdFunction>("USER", &user));
	_cmdList.insert(std::pair<const std::string, cmdFunction>("JOIN", &join));
	_cmdList.insert(std::pair<const std::string, cmdFunction>("PRIVMSG", &privmsg));
	_cmdList.insert(std::pair<const std::string, cmdFunction>("MODE", &mode));
	_cmdList.insert(std::pair<const std::string, cmdFunction>("INVITE", &invite));
	_cmdList.insert(std::pair<const std::string, cmdFunction>("TOPIC", &topic));
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

std::map<const int, Client> &Server::getClients() { return (_clients); }

std::string const &Server::getPass() { return (_password); }

std::map<const std::string, Channel> &Server::getChannels() { return (_channels); }

char	*Server::getCreationDate() { return (_creationDate); }

void Server::addChannel(Channel channel) {

	_channels.insert(std::pair<const std::string, Channel>(channel.getName(), channel));
}

void Server::start() {

	errno = 0;

	addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status = getaddrinfo(NULL, _port, &hints, &_servInfo);
	if (status != 0) {
		std::cout << RED << ERR_GETADDINFO << gai_strerror(status) << RESET << std::endl;
		exit (1);
	}

	_socket = socket(_servInfo->ai_family, _servInfo->ai_socktype, _servInfo->ai_protocol);
	if (_socket < 0) {
		std::perror(ERR_SOCKET);
		freeaddrinfo(_servInfo);
		exit (1);
	}

	int optValue = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(optValue)) != 0) {
		std::perror("setsockopt");
		freeaddrinfo(_servInfo);
		exit (1);
	}

	bind(_socket, _servInfo->ai_addr, _servInfo->ai_addrlen);
	listen(_socket, 10);
	freeaddrinfo(_servInfo);
	if (errno != 0) {
		std::perror(ERR_START_SERV);
		exit (1);
	}

	std::time_t currentTime = std::time(NULL);
	std::strftime(_creationDate, sizeof(_creationDate), "%H:%M:%S %m-%d-%Y", std::localtime(&currentTime));


	std::cout << GREEN << "Server #" << _socket << " listening on port " << _port << RESET << std::endl;
	std::cout << GREEN << "Time : " << _creationDate << RESET << std::endl;

	launchServLoop();
}

void Server::launchServLoop() {

	std::vector<pollfd>	pfds;
	pollfd				server_pfd;

	server_pfd.fd = _socket;
	server_pfd.events = POLLIN;
	pfds.push_back(server_pfd);

	while (1) {
		std::vector<pollfd> newPfds;

		int events = poll((pollfd *)&pfds[0], (unsigned int) pfds.size(), -1);
		if (events < 0) {
			std::cerr << ERR_POLL << std::endl;
			exit (1);
		}

		int eventsFound = 0;
		std::vector<pollfd>::iterator it = pfds.begin();
		while (it != pfds.end() && eventsFound < events) {
			if (it->revents & POLLIN) { // if the event is a POLLIN -> the socket is ready to recv data
				eventsFound++;
				if (it->fd == _socket) // if the socket is the server one, we add a new client
					newClient(pfds, newPfds);
				else { // else if the socket is that of an existing client
					{
						Client &client = findClient(this, it->fd);
						std::cout << "Register : " << client.getIsRegistered() << std::endl;
						manageExistingConnection(pfds, it);

					}// handle already existing connection
				}
			}
			else if (it->revents & POLLOUT) { // if I can send data to the socket
				eventsFound++;
				managePollout(pfds, it);
			}
			it++;
		}
		pfds.insert(pfds.end(), newPfds.begin(), newPfds.end());
	}
}

void Server::newClient(std::vector<pollfd> pfds, std::vector<pollfd> &newPfds) {
	sockaddr_in client;
	socklen_t addr_size = sizeof(sockaddr_in);

	int clientSocket = accept(_socket, (sockaddr *)&client, &addr_size);
	if (clientSocket < 0)
		std::cerr << ERR_ACCEPT << std::endl;
	if (pfds.size() - 1 < MAX_CLIENTS) { // if server isn't full
		pollfd	clientPfd;
		Client	newClient(clientSocket);

		clientPfd.fd = clientSocket;
		clientPfd.events = POLLIN | POLLOUT;
		newPfds.push_back(clientPfd);
		_clients.insert(std::pair<const int, Client>(clientSocket, newClient));

		std::cout << "Register : " << newClient.getIsRegistered() << std::endl;
		std::cout << BLUE << "Server added client #" << clientSocket << RESET << std::endl;

	}
	else {
		std::cout << RED << ERR_FULL_SERV << RESET << std::endl;
		send(clientSocket, ERR_FULL_SERV, strlen(ERR_FULL_SERV) + 1, MSG_NOSIGNAL);
		close(clientSocket);
	}
}

void Server::manageExistingConnection(std::vector<pollfd> &pfds, std::vector<pollfd>::iterator &it) {

	Client &client = findClient(this, it->fd);
	char	msg[BUFF_SIZE];

	memset(msg, 0, sizeof(msg));
	int	readCount = recv(it->fd, msg, BUFF_SIZE, 0);

	pfds.end();
	if (readCount < 0) {
		std::cerr << RED << ERR_RCV << RESET << std::endl;
		delClient(&pfds, it);
		return;
	}
	else if (readCount == 0) {
		std::cout << YELLOW << "[Server] Client #" << it->fd << " just disconnected" << RESET << std::endl;
		delClient(&pfds, it);
		_clients.erase(it->fd);
		return;
	}
	else {
		std::cout << BLUE << "[Client] Message received from client #" << it->fd << RESET << " " << msg << std::endl;
		client.setReadBuff(msg);
		if (client.getReadBuff().find("\r\n") == std::string::npos)
			client.setReadBuff("\r\n");
		Message	msgRead(msg);

		if (client.getReadBuff().find("\r\n") != std::string::npos)
		{
			parseMsg(it->fd, msgRead); // parse readBuff to find cmds, if client isn't registered see for NICK etc...
			if (client.getReadBuff().find("\r\n") != std::string::npos)
				client.getReadBuff().clear();
		}
	}
}

void	Server::managePollout(std::vector<pollfd> &pfds, std::vector<pollfd>::iterator &it)
{
	Client &client = findClient(this, it->fd);

	(void) pfds;
	/*if (!client)
			std::cerr << RED << "[Server] Can't find the client" << RESET << std::endl;*/
	//else
//	{
		sendMsg(it->fd, client.getSendBuff());
		client.getSendBuff().clear();
//	}
}
