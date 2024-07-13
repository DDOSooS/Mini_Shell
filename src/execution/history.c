#include "../../includes/mshell.h"

void put_tohistory(char *cmd, t_history *history)
{
	t_history *tmp;

	tmp = history;
	if (!cmd)
		return ;
	if (!tmp->cmd)
	{
		tmp->cmd = ft_strdup(cmd);
		tmp->id = 1;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = (t_history *)malloc(sizeof(t_history));
	tmp->next->cmd = ft_strdup(cmd);
	tmp->next->id = tmp->id + 1;
	tmp->next->next = NULL;
}
