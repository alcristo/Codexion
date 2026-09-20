NAME = codexion

SOURCES =	srcs/simulation/main.c \
			srcs/parsing/parsing.c \
			srcs/simulation/simulation.c \
			srcs/simulation/permission.c \
			srcs/utils/codexion_utils.c \
			srcs/coder/init_coders.c \
			srcs/coder/coder_actions.c \
			srcs/dongle/init_dongles.c \
			srcs/dongle/dongle_actions.c \
			srcs/heap/init_heap.c \
			srcs/heap/heap_ops.c \
			srcs/logger/init_logger.c \
			srcs/logger/log_ops.c \

OBJECTS = $(SOURCES:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread  -g -fsanitize=thread
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
