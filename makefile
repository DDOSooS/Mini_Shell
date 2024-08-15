NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -pedantic -fsanitize=address
LIBS = -lreadline
LIBFT = includes/libft/libft.a
PRINTF = includes/printf/libftprintf.a

SRC = src/main.c\
		src/parsing/syntax-error.c\
		src/parsing/tokenizer.c\
		src/parsing/parser.c\
		src/execution/builtins.c\
		src/execution/execute.c\
		src/execution/cmd_runner.c\
		src/execution/cmd_runner_utils1.c\
		src/execution/cmd_runner_utils2.c\
		src/execution/redirections.c\
		src/execution/signals.c\
		src/execution/environ.c\
		src/execution/environ_utils.c\
		src/execution/history.c\
		src/parsing/expander.c\
		src/execution/freeier.c\
		src/execution/printer.c\
		src/execution/pipes.c\
		src/execution/herdoc.c\
		includes/gnl/get_next_line.c\
		includes/gnl/get_next_line_utils.c\
		src/execution/builtins/ft_cd.c\
		src/execution/builtins/ft_env.c\
		src/execution/builtins/ft_pwd.c\
		src/execution/builtins/ft_exit.c\
		src/execution/builtins/ft_echo.c\
		src/execution/builtins/ft_export_utils.c\
		src/execution/builtins/ft_export.c\
		src/execution/builtins/ft_unset.c\
		src/execution/builtins_utils.c\
		src/execution/wildcards.c\

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(PRINTF)
	$(CC) $(CFLAGS) -g3 $^ -o $@ $(LIBS)
# $(CC) $(CFLAGS) -fsanitize=address $^ -o $@ $(LIBS)

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

sup: all
	@echo "Running valgrind"
	valgrind -s --leak-check=full --suppressions=readline.supp ./minishell

.PHONY: all clean fclean re
.SECONDARY: $(OBJ)
