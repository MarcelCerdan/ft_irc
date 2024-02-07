#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

class Client;
class Message;
class Channel;

class Server {

private:
	char						*_port;
	int							_socket;
	const std::string			_password;
	addrinfo					*_servInfo;
	std::map<const int, Client>	_clients;
	std::map<const std::string, void (*)(Server *, Message, int)>	_cmdList;
	std::map<const std::string, Channel>	_channels;

public:
	Server(char *port, const std::string &password);
	Server(const Server &other);
	~Server();

	Server &operator=(const Server &other);

	std::map<const int, Client> &getClients();
	std::string const &getPass();
	std::map<const std::string, Channel> &getChannels();

	void	addChannel(std::string const &name, Channel &channel);

	void	launchServLoop();
	void	start();
	void	newClient(std::vector<pollfd> pfds, std::vector<pollfd> &newPfds);
	void	manageExistingConnection(std::vector<pollfd> &pfds, std::vector<pollfd>::iterator &it);
	int		parseMsg(int clientFd, class Message &msg);
	void	registerClient(Message	&msg, int	clientFd);
	void	managePollout(std::vector<pollfd> &pfds, std::vector<pollfd>::iterator &it);

};

Client *findClient(Server *server, int fd);
Client &getClient(Server *server, int fd);
void	addToClientBuf(Server *serv, int clientFd, std::string str);
void	delClient(std::vector<pollfd> *pfds, std::vector<pollfd>::iterator it);

#endif
