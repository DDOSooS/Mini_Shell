/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:33:31 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:33:32 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	add_env(t_env *env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = env;
	new = (t_env *)malloc(sizeof(t_env));
	new->key = key;
	new->value = value;
	if (!value)
		new->is_exported = 0;
	else
		new->is_exported = 1;
	new->next = NULL;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	builtins_checker(t_cmd *cmd)
{
	if (ft_strcmp(cmd->arg, "cd") == 0)
		return (CD);
	if (ft_strcmp(cmd->arg, "echo") == 0)
		return (ECHO);
	if (ft_strcmp(cmd->arg, "pwd") == 0)
		return (PWD);
	if (ft_strcmp(cmd->arg, "export") == 0)
		return (EXPORT);
	if (ft_strcmp(cmd->arg, "unset") == 0)
		return (UNSET);
	if (ft_strcmp(cmd->arg, "env") == 0)
		return (ENV);
	if (ft_strcmp(cmd->arg, "exit") == 0)
		return (EXIT);
	if (ft_strcmp(cmd->arg, "history") == 0)
		return (HISTORY);
	return (-1);
}

int	builtins_finder(t_cmd *cmd, t_mshell *shell, int type)
{
	if (type == CD)
		return (ft_cd(cmd, shell));
	if (type == ECHO)
		return (ft_echo(cmd, shell));
	if (type == PWD)
		return (ft_pwd(cmd, shell));
	if (type == EXPORT)
		return (ft_export(cmd, shell));
	if (type == UNSET)
		return (ft_unset(cmd, shell));
	if (type == ENV)
		return (ft_env(cmd, shell));
	if (type == EXIT)
		return (ft_exit(cmd, shell));
	if (type == HISTORY)
		return (ft_history(cmd, shell));
	return (-1);
}
