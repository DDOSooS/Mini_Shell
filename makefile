NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -pedantic
LIBS = -lreadline
LIBFT = includes/libft/libft.a
PRINTF = includes/printf/libftprintf.a
SRC = src/main.c \
		src/parsing/lexer/syntax_error.c\
		src/parsing/lexer/lexer_cheker.c\
		src/parsing/lexer/lexer_cheker_utils.c\
		src/parsing/lexer/lexer_cheker_utils1.c\
		src/parsing/lexer/lexer_cheker_utils2.c\
		src/parsing/lexer/syntax_cheker.c\
		src/parsing/lexer/syntax_error_utils.c\
		src/parsing/lexer/syntax_error_utils1.c\
		src/parsing/lexer/syntax_error_utils2.c\
		src/parsing/lexer/syntax_error_utils3.c\
		src/parsing/tokenizer/tokenizer.c\
		src/parsing/tokenizer/tokenizer_checker.c \
		src/parsing/expander/expander.c\
		src/parsing/expander/precised_split.c \
		src/parsing/expander/expander_checker.c \
		src/parsing/expander/expand_generator.c \
		src/parsing/expander/expand_token_len_counter.c \
		src/parsing/expander/expand_tokens.c \
		src/parsing/expander/expander_generator_utils.c \
		src/parsing/expander/herdoc_generater.c \
		src/parsing/parser/parser.c\
		src/parsing/parser/parser_utils.c\
		src/parsing/parser/cmd_generator.c\
		src/parsing/parser/new_node_generator.c\
		src/parsing/parser/parser_cheker.c\
		src/parsing/parser/parser_cheker_utils.c\
		src/parsing/parser/redirection_generator.c\
		src/execution/builtins.c\
		src/execution/execute.c\
		src/execution/cmd_runner.c\
		src/execution/cmd_runner_utils1.c\
		src/execution/cmd_runner_utils2.c\
		src/execution/redirections.c\
		src/execution/signals.c\
		src/execution/environ.c\
		src/execution/environ_utils.c\
		src/execution/executer/history.c\
		src/execution/freeier.c\
		src/execution/printer.c\
		src/execution/executer/pipes.c\
		src/execution/executer/exec_cases.c\
		src/execution/executer/herdoc.c\
		src/execution/executer/herdoc_utils.c\
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
	valgrind -s --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
.PHONY: all clean fclean re
.SECONDARY: $(OBJ)
