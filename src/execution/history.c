#include "../../includes/mshell.h"

void put_tohistory(char *cmd, t_history *history)
{
	t_history *tmp;

	tmp = history;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = (t_history *)malloc(sizeof(t_history));
	tmp->next->id = tmp->id + 1;
	tmp->next->cmd = ft_strdup(cmd);
	tmp->next->next = NULL;
}