#include "../../../includes/mshell.h"

int	ft_env(t_cmd *cmd, t_mshell *shell)
{
	t_env	*tmp;

	(void)cmd;
	tmp = shell->env;
	while (tmp)
	{
		if (tmp->value)
		{
			if (ft_strcmp(tmp->key, "_") !=	0)
				printf("%s=%s\n", tmp->key, tmp->value);
		}
		tmp = tmp->next;
	}
	printf("%s\n", "_=/user/bin/env");
	return (0);
}
