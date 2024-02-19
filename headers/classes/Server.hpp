#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

class Client;
class Message;
class Channel;

class Server {

private:
	char 						_creationDate[100];
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
	char	*getCreationDate();

	void	addChannel(Channel &channel);

	void	launchServLoop();
	void	start();
	void	newClient(std::vector<pollfd> pfds, std::vector<pollfd> &newPfds);
	void	manageExistingConnection(std::vector<pollfd> &pfds, std::vector<pollfd>::iterator &it);
	void	registerClient(Message	&msg, int	clientFd);
	void	managePollout(std::vector<pollfd> &pfds, std::vector<pollfd>::iterator &it);
	void	delClient(std::vector<pollfd> *pfds, std::vector<pollfd>::iterator it);
	int		parseMsg(int clientFd, class Message &msg);

};

Client	&findClient(Server *server, int fd);
Client	&getClient(Server *serv, const std::string &nick);
Channel &findChannel(Server *serv, const std::string &chanName);
void	addToClientBuf(Server *serv, int clientFd, std::string str);
void	printServInfo(Server *serv, int clientFd);

#endif
