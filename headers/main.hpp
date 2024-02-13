#ifndef MAIN_HPP
# define MAIN_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <cstring>
# include <string>
# include <cstdlib>
# include <cstdio>
# include <cerrno>
# include <vector>
# include <map>
# include <arpa/inet.h>
# include <poll.h>
# include <unistd.h>
# include <csignal>

# include "errorMessages.hpp"
# include "colors.hpp"
# include "classes/Server.hpp"
# include "classes/Client.hpp"
# include "classes/Message.hpp"
# include "numericReplies.hpp"
# include "commands.hpp"
# include "classes/Channel.hpp"

# define MAX_CLIENT 10
# define BUFF_SIZE	512
# define MIN_PORT	1025
# define MAX_PORT	65535

typedef void (*cmdFunction)(Server *, Message, int);

#endif
