#include "../../../includes/mshell.h"

static int	exit_value_check(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	exit_handler(char *arg)
{
	int		exit_value;
	char	*buffer;

	if (!exit_value_check(arg))
	{
		buffer = ft_strjoin("minishell: exit: ", arg);
		write(2, buffer, ft_strlen(buffer));
		write(2, ": numeric argument required\n", 28);
		free(buffer);
		free_gvar();
		return (2);
	}
	exit_value = ft_atoi(arg);
	if (exit_value < 0)
		exit_value = 256 + exit_value;
	free_gvar();
	return (exit_value);
}

int	ft_exit(t_cmd *cmd, t_mshell *shell)
{
	t_cmd	*tmp;
	// int		exit_value;

	tmp = NULL;
	(void)shell;
	if (cmd->next != NULL)
		tmp = cmd->next;
	write(2, "exit\n", 5);
	if (tmp && tmp->next != NULL)
	{
		print_stderr("exit: too many arguments");
		return (1);
	}
	if (tmp && tmp->arg)
		exit(exit_handler(tmp->arg));
	else
	{
		free_gvar();
		exit(g_mshell.exit_value);
	}
	return (0);
}
