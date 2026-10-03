NAME = codexion

SOURCES =	srcs/coder_actions.c \
			srcs/dongle_actions.c \
			srcs/heap_ops.c \
			srcs/heap_utils.c \
			srcs/init_coders.c \
			srcs/init_heap.c \
			srcs/logger_ops.c \
			srcs/main.c \
			srcs/parsing.c \
			srcs/permission.c \
			srcs/preparatives.c \
			srcs/routine.c \
			srcs/simulation.c \
			srcs/time.c \

OBJECTS = $(SOURCES:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

%.o:%.c
	@$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

$(OBECTS):
	$(CC) $(CFLAGS) -c -o $(OBJECTS) $(SOURCES)
	
clean: 
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
