# include "../../includes/mshell.h"

void free_env(t_env *env)
{
	t_env *tmp;
	t_env *tmp2;

	tmp = env;
	while (tmp)
	{
		tmp2 = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = tmp2;
	}
}

void free_history(t_history *history)
{
	t_history *tmp;
	t_history *tmp2;

	tmp = history;
	while (tmp)
	{
		tmp2 = tmp->next;
		free(tmp->cmd);
		free(tmp);
		tmp = tmp2;
	}
}

void free_gvar(void)
{
	free_env(g_mshell.env);
	free_history(g_mshell.history);
}
