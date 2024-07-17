#include "../../../includes/mshell.h"

static int	echo_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' && arg[i + 1] == 'n')
	{
		i++;
		while (arg[i] == 'n')
			i++;
	}
	if (arg[i] == '\0')
		return (1);
	return (0);
}

static void	print_args(t_cmd *tmp, int flag)
{
	tmp = tmp->next;
	if (flag == 1)
		tmp = tmp->next;
	while (tmp)
	{
		printf("%s", tmp->arg);
		if (tmp->next)
			printf(" ");
		tmp = tmp->next;
	}
}

int	ft_echo(t_cmd *cmd, t_mshell *shell)
{
	t_cmd	*tmp;
	int		flag;

	tmp = cmd;
	flag = 0;
	(void)shell;
	if (tmp->next && tmp->next->arg)
	{
		flag = echo_arg(tmp->next->arg);
		print_args(tmp, flag);
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
