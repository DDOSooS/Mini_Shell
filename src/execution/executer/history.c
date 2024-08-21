#include "../../../includes/mshell.h"

static int	check_last_command(char *cmd, t_history *history)
{
	t_history	*tmp;

	tmp = history;
	if (!tmp->cmd)
		return (0);
	while (tmp->next)
		tmp = tmp->next;
	if (ft_strcmp(tmp->cmd, cmd) == 0)
		return (1);
	return (0);
}

void	put_tohistory(char *cmd, t_history *history, int herdoc)
{
	t_history	*tmp;

	tmp = history;
	if (!cmd)
		return ;
	if (!tmp->cmd)
	{
		tmp->cmd = ft_strdup(cmd);
		tmp->herdoc = herdoc;
		tmp->id = 1;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	if (check_last_command(cmd, history))
		return ;
	tmp->next = (t_history *)malloc(sizeof(t_history));
	tmp->next->cmd = ft_strdup(cmd);
	tmp->next->id = tmp->id + 1;
	tmp->next->herdoc = herdoc;
	tmp->next->next = NULL;
}

int	ft_history(t_cmd *cmd, t_mshell *shell)
{
	t_history	*tmp;

	(void)cmd;
	tmp = shell->history;
	while (tmp && tmp->cmd)
	{
		if (!tmp->herdoc)
			printf(" %d  %s\n", tmp->id, tmp->cmd);
		else
			printf("%s\n", tmp->cmd);
		tmp = tmp->next;
	}
	return (0);
}
