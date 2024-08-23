/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeier.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:13 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/23 11:18:06 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	ft_free_tree(t_tnode **tree)
{
	if (*tree)
	{
		ft_free_tree(&(*tree)->t_left);
		ft_free_tree(&(*tree)->t_right);
		if ((*tree)->cmd)
			ft_free_cmds((*tree)->cmd);
		if ((*tree)->redirection)
			ft_free_redirections((*tree)->redirection);
		free(*tree);
		*tree = NULL;
	}
}

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

void	free_gvar(int flag)
{
	free_env(g_mshell.env);
	free_history(g_mshell.history);
	if (flag)
		ft_free_herdoc(&g_mshell.herdocs);
	ft_free_tokens(&g_mshell.token);
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
