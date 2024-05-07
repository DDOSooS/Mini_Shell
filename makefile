NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -pedantic
LIBS = -lreadline
LIBFT = includes/libft/libft.a

SRC = src/main.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C includes/libft

clean:
	rm -f $(OBJ)
	$(MAKE) -C includes/libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C includes/libft fclean

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJ)
