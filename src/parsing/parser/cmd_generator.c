/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_generator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:03:55 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 12:24:50 by aghergho         ###   ########.fr       */
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

void	ft_trim_cmd(char **cmd)
{
	int		i;
	int		start;
	char	*new;

	if (!cmd || !*cmd)
		return ;
	i = 0;
	start = 0;
	while ((*cmd)[i] && is_white_space((*cmd)[i]))
		i++;
	start = i;
	i = ft_strlen(*cmd) - 1;
	while (i >= start && is_white_space((*cmd)[i]))
		i--;
	if (i < start)
		new = ft_strdup("");
	else
		new = ft_substr(*cmd, start, i - start + 1);
	free(*cmd);
	*cmd = new;
}

int	ft_add_to_cmd(t_cmd **root, char *token)
{
	char	*cmd;
	t_cmd	*tmp;
	t_cmd	*new;

	tmp = *root;
	cmd = ft_strdup(token);
	ft_expand_arg(&cmd, 0);
	ft_trim_cmd(&cmd);
	new = malloc(sizeof(t_cmd));
	new->arg = cmd;
	new->check_wildcard = 0;
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
