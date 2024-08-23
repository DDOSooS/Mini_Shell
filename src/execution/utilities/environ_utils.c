/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:09 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:34:10 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static void	swap_envs(t_env *env1, t_env *env2)
{
	char	*tmp_key;
	char	*tmp_value;
	int		tmp_export;

	tmp_key = env1->key;
	tmp_value = env1->value;
	tmp_export = env1->is_exported;
	env1->key = env2->key;
	env1->value = env2->value;
	env1->is_exported = env2->is_exported;
	env2->key = tmp_key;
	env2->value = tmp_value;
	env2->is_exported = tmp_export;
}

t_env	*sort_env(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = env;
	while (tmp)
	{
		tmp2 = tmp->next;
		while (tmp2)
		{
			if (ft_strcmp(tmp->key, tmp2->key) > 0)
				swap_envs(tmp, tmp2);
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	return (env);
}

static t_env	*copy_env_node(t_env *node)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (node->key)
		new_node->key = ft_strdup(node->key);
	else
		new_node->key = NULL;
	if (node->value)
		new_node->value = ft_strdup(node->value);
	else
		new_node->value = NULL;
	new_node->is_exported = node->is_exported;
	new_node->next = NULL;
	return (new_node);
}

t_env	*copy_env(t_env *env)
{
	t_env	*tmp;
	t_env	*new;
	t_env	*head;

	if (env == NULL)
		return (NULL);
	tmp = env;
	head = copy_env_node(tmp);
	new = head;
	tmp = tmp->next;
	while (tmp)
	{
		new->next = copy_env_node(tmp);
		new = new->next;
		tmp = tmp->next;
	}
	return (head);
}
