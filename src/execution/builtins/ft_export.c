/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:32:54 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/23 10:14:27 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static int	export_checker(t_env *new, char *arg)
{
	int	i;
	int	plus;

	(void)(arg);
	i = 0;
	plus = 0;
	while (new && new->key[i])
	{
		if (!ft_isalnum(new->key[i]) && new->key[i] != '_'
			&& new->key[i] != '+')
			return (export_erorr(new->key, 1));
		if (new->key[i] == '+')
			plus++;
		if (plus > 1 || new->key[0] == '+' || (new->key[0] >= '0'
				&& new->key[0] <= '9'))
			return (export_erorr(new->key, 1));
		i++;
	}
	if ((new->key && new->key[0] == '\0') || (new->key[ft_strlen(new->key)
				- 1] == '+' && new->value == NULL) || (plus == 1
			&& new->key[ft_strlen(new->key) - 1] != '+'))
		return (export_erorr(new->key, 1));
	return (0);
}

static int	print_export(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = copy_env(env);
	tmp = sort_env(tmp);
	tmp2 = tmp;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "_") != 0 && tmp->is_exported != 2)
		{
			if (tmp->value)
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			else
				printf("declare -x %s\n", tmp->key);
		}
		tmp = tmp->next;
	}
	free_env(tmp2);
	return (0);
}

static void	add_append_env(t_env *env, t_env *new)
{
	if (ft_strchr(new->key, '+'))
		appned_export(env, new);
	else
		replacement_export(env, new);
}

static int	add_export(t_cmd *cmd, t_mshell *shell)
{
	t_env	*new;
	int		status;
	int		returned_status;

	returned_status = 0;
	while (cmd && cmd->arg && cmd->arg[0])
	{
		status = 0;
		new = extarct_node(cmd->arg);
		status = export_checker(new, cmd->arg);
		if (status)
		{
			free_env(new);
			cmd = cmd->next;
			returned_status = 1;
			continue ;
		}
		add_append_env(shell->env, new);
		cmd = cmd->next;
	}
	return (returned_status);
}

int	ft_export(t_cmd *cmd, t_mshell *shell)
{
	t_cmd	*tmp;

	if (cmd->next == NULL)
		return (print_export(shell->env));
	tmp = cmd->next;
	return (add_export(tmp, shell));
}
