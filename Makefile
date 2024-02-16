.PHONY:	all clean fclean re

# ******** VARIABLES ******** #

# ---- Final Executable --- #

NAME		=	ircserv

# ---- Directories ---- #

DIR_OBJS	=	.objs/

DIR_SRC		= 	srcs/

DIR_HEAD 	=	headers/

# ---- Files ---- #

HEAD_LIST	=	main.hpp \
				classes/Server.hpp \
				classes/Client.hpp \
				classes/Message.hpp \
				classes/Channel.hpp \
				errorMessages.hpp \
				colors.hpp \
				commands.hpp \
				numericReplies.hpp

SRCS_LST 	= 	main.cpp \
				classes/Server.cpp \
				classes/Client.cpp \
				classes/Message.cpp \
				classes/Channel.cpp \
				utils.cpp \
				parseMsg.cpp \
				commands/pass.cpp \
				commands/nick.cpp \
				commands/user.cpp \
				commands/join.cpp \
				commands/privmsg.cpp \
				commands/mode.cpp \
				commands/invite.cpp

OBJS 		= 	$(addprefix $(DIR_OBJS), $(SRCS_LST:.cpp=.o))

DEPS		=	$(OBJS:.o=.d)


# ---- Compilation ---- #

CC		=	c++
CFLAGS	=	-Wall -Wextra -Werror -std=c++98

# ---- Commands ---- #

RM		=	rm -rf
MKDIR	=	mkdir -p
DFLAG	=	-MMD -MP

# ********* RULES ******** #

.PHONY:				all
all			:	${NAME}

-include		$(DEPS)

# ---- Variables Rules ---- #

${NAME}			:	${OBJS}
					${CC} ${CFLAGS} ${OBJS} -o ${NAME}

# ---- Compiled Rules ---- #

$(DIR_OBJS)%.o	:	$(DIR_SRC)%.cpp
					${MKDIR} $(shell dirname $@)
					${CC} ${CFLAGS} $(DFLAG) -I $(DIR_HEAD) -c $< -o $@

# ---- Usual Commands ---- #

.PHONY:				clean
clean			:
					${RM} ${DIR_OBJS}

.PHONY:				fclean
fclean			:	clean
					${RM} ${NAME}

.PHONY:				re
re				:	fclean
					$(MAKE) all