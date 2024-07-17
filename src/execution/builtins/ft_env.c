#include "../../../includes/mshell.h"

int	ft_env(t_cmd *cmd, t_mshell *shell)
{
	t_env	*tmp;

	(void)cmd;
	tmp = shell->env;
	while (tmp)
	{
		if (tmp->is_exported)
		{
			if (tmp->value)
				printf("%s=%s\n", tmp->key, tmp->value);
			else
				printf("%s=\n", tmp->key);
		}
		tmp = tmp->next;
	}
	ft_printf("_=/usr/bin/env\n");
	return (0);
}
