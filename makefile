NAME = minishell

CC = cc
#CFLAGS = -Wall -Wextra -pedantic
LIBS = -lreadline
LIBFT = includes/libft/libft.a
PRINTF = includes/printf/libftprintf.a

SRC = src/main.c\
		src/parsing/syntax-error.c\
		src/parsing/tokenizer.c\
		src/parsing/parser.c\
		src/execution/execute.c\
		src/execution/signals.c\

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(PRINTF)
	$(CC) $(CFLAGS)  $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C includes/libft
$(PRINTF) :
	$(MAKE) -C includes/printf
clean:
	rm -f $(OBJ)
	$(MAKE) -C includes/libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C includes/libft fclean

re: fclean all	

.PHONY: all clean fclean re
.SECONDARY: $(OBJ)
