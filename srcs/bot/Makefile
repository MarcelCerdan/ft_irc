.PHONY:	all clean fclean re

# ******** VARIABLES ******** #

# ---- Final Executable --- #

NAME		=	bot

# ---- Directories ---- #

DIR_OBJS	=	.objs/

DIR_SRC		= 	srcs/

DIR_HEAD 	=	header/

# ---- Files ---- #

HEAD_LIST	=	bot.hpp \

SRCS_LST 	= 	main.cpp \

OBJS 		= 	$(addprefix $(DIR_OBJS), $(SRCS_LST:.cpp=.o))

DEPS		=	$(OBJS:.o=.d)


# ---- Compilation ---- #

CC		=	c++
CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g3

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