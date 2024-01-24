#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

# include "main.hpp"

class Client {

private:
	const int	_socket;

public:
	Client(const int socket);
	Client(const Client &other);
	~Client();

	Client &operator=(const Client &other);

};


#endif //FT_IRC_CLIENT_HPP
