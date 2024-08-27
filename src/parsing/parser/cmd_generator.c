/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_generator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:03:55 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/24 19:42:11 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_add_cmd(t_cmd **cmd, char *str)
{
	t_cmd	*new;
	t_cmd	*tmp;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (0);
	if (str)
		new->arg = ft_strdup(str);
	else
		new->arg = NULL;
	new->check_wildcard = ft_check_wild_card(new->arg);
	new->next = NULL;
	if (!cmd || !*cmd)
		*cmd = new;
	else
	{
		tmp = *cmd;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}

int	ft_add_to_cmd(t_cmd **root, char *token)
{
	t_cmd	*tmp;
	t_cmd	*new;

	tmp = *root;
	new = malloc(sizeof(t_cmd));
	if (token)
	{
		new->arg = ft_substr(token, 1, ft_strlen(token) - 2);
		free(token);
	}
	else
		new->arg = NULL;
	new->check_wildcard = ft_check_wild_card(new->arg);
	new->next = NULL;
	if (!root || !*root)
	{
		*root = new;
		return (1);
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (1);
}

t_cmd	*ft_gen_new_cmds(char *arg)
{
	t_cmd	*new;
	char	**cmds;
	int		i;

	i = 0;
	new = NULL;
	cmds = ft_split_words(arg, " \t");
	while (cmds[i])
	{
		ft_add_cmd(&new, cmds[i]);
		free(cmds[i]);
		i++;
	}
	free(cmds);
	return (new);
}

t_cmd	*ft_split_cmd(char *arg)
{
	t_cmd	*new;

	new = ft_gen_new_cmds(arg);
	free(arg);
	return (new);
}
