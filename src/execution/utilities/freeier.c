#include "../../../includes/mshell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = env;
	while (tmp)
	{
		tmp2 = tmp->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
		tmp = tmp2;
	}
}

void	free_history(t_history *history)
{
	t_history	*tmp;
	t_history	*tmp2;

	tmp = history;
	while (tmp)
	{
		tmp2 = tmp->next;
		if (tmp->cmd)
			free(tmp->cmd);
		free(tmp);
		tmp = tmp2;
	}
}
/*
void free_herdoc(t_herdoc *herdoc)
{
	t_herdoc *tmp;
	t_herdoc *tmp2;

	tmp = herdoc;
	while (tmp)
	{
		tmp2 = tmp->next;
		if (tmp->delimiter)
			free(tmp->delimiter);
		free(tmp);
		tmp = tmp2;
	}
}
*/

void	free_gvar(void)
{
	free_env(g_mshell.env);
	free_history(g_mshell.history);
	ft_free_tokens(&g_mshell.token);
	ft_free_herdoc(&g_mshell.herdocs);
	ft_free_tree(&g_mshell.cmd_tree);
}

void	free_func(char **strings)
{
	int	x;

	x = -1;
	while (strings[++x])
		free(strings[x]);
	free(strings);
}
