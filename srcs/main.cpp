#include "classes/Server.hpp"

bool	checkArgs(int ac, char **av);
void	signalHandler(int signal);

int	main(int ac, char **av) {
	if (!checkArgs(ac, av))
		return (0);

	std::string password = av[2];

	signal(SIGINT, signalHandler);
	signal(SIGTSTP, signalHandler);

	Server server(av[1], password);
	server.start();
	close(3);
	return (0);
}

void	signalHandler(int signal) {
	if (signal == SIGINT) // shutdown the server
		return ;
	else if (signal == SIGTSTP)
		std::cout << "Received SIGTSTP (CTRL+Z)" << std::endl;
}

bool	checkArgs(int ac, char **av) {
	if (ac != 3) {
		std::cout << RED << ERR_ARG_NMB << RESET << std::endl;
		return (false);
	}

	for (int i = 0; av[1][i]; i++) {
		if (!isdigit(av[1][i])) {
			std::cout << RED << ERR_PORT_VALUE << RESET << std::endl;
			return (false);
		}
	}

	if (atoi(av[1]) < MIN_PORT || atoi(av[1]) > MAX_PORT) {
		std::cout << RED << ERR_PORT_VALUE << RESET << std::endl;
		return (false);
	}
	return (true);
}