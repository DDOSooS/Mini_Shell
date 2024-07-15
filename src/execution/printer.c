# include "../../includes/mshell.h"

void print_stderr(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
}