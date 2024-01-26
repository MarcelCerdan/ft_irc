#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

# include "main.hpp"

class Client {

private:
	const int	_socket;
	std::string _readBuff;
	std::string _nickname;
	std::string	_username;
	bool		_goodPass;
	bool		_isRegistered;

public:
	Client(const int socket);
	Client(const Client &other);
	~Client();

	Client &operator=(const Client &other);

	void		setReadBuff(std::string const &msg);
	std::string &getReadBuff();

};


#endif //FT_IRC_CLIENT_HPP
