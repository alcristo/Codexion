NAME = codexion

SOURCES = src/*.c

OBJECTS = $(SOURCES:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

%.o:%.c
	@$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LIBS)
	
clean: 
	$(RM) $(OBJECTS) $(LIBS) $(OBJECTS_CHECKER)

fclean: clean
	$(RM) $(NAME) $(NAME_CHECKER)

re:	fclean all

.PHONY: all clean fclean re
