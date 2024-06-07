#include "../../includes/mshell.h"

int ft_env(t_env *env)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			ft_printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

int ft_exit(char *cmd)
{
	(void)cmd;
	exit(0);
	return (0);
}
int 

int builtins_finder(char *cmd)
{
	int flag;
	int i;
	t_builtins builtins[] = {
		{"cd", &ft_cd},
		{"echo", &ft_echo},
		{"pwd", &ft_pwd},
		{"export", &ft_export},
		{"unset", &ft_unset},
		{"env", &ft_env},
		{"exit", &ft_exit},
		{NULL, NULL}
	};

	i = 0;
	while (builtins[i].cmd)
	{
		if (ft_strcmp(builtins[i].cmd, cmd) == 0)
			flag = builtins[i].func(cmd);
			break;
		i++;
	}
	return (flag);
}
