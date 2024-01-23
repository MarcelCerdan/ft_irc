#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

class Server {

private:
	char				*_port;
	int					_socket;
	const std::string	_password;
	addrinfo			*_servInfo;
	pollfd				*_pfds[10];

public:
	Server(char *port, const std::string &password);
	Server(const Server &other);
	~Server();

	Server &operator=(const Server &other);

	void launchServLoop() const;
	void start();
};

#endif
