#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"
# include "Client.hpp"

class Server {

private:
	char						*_port;
	int							_socket;
	const std::string			_password;
	addrinfo					*_servInfo;
	std::map<const int, Client>	_clients;

public:
	Server(char *port, const std::string &password);
	Server(const Server &other);
	~Server();

	Server &operator=(const Server &other);

	void	launchServLoop();
	void	start();
	int		newClient(std::vector<pollfd> pfds, std::vector<pollfd> newPfds);
};

#endif
