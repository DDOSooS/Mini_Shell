/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:33:19 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/23 10:14:27 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

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
	tmp->next = (t_history *)malloc(sizeof(t_history));
	tmp->next->cmd = ft_strdup(cmd);
	if (herdoc == 1)
		tmp->next->id = tmp->id;
	else
		tmp->next->id = tmp->id + 1;
	tmp->next->herdoc = herdoc;
	tmp->next->next = NULL;
}

int	ft_history(t_cmd *cmd, t_mshell *shell)
{
	t_history	*tmp;

	(void)(cmd);
	tmp = shell->history;
	while (tmp && tmp->cmd)
	{
		if (!tmp->herdoc)
			printf("  %d  %s\n", tmp->id, tmp->cmd);
		else
			printf("%s\n", tmp->cmd);
		tmp = tmp->next;
	}
	return (0);
}
