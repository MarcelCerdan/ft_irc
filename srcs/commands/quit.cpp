#include "main.hpp"

void quit(Server *serv, Message msg, int clientFd) {
	static_cast<void>(serv);
	static_cast<void>(msg);
	static_cast<void>(clientFd);
}