/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:05 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/28 17:59:27 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

t_env	*find_env(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp != NULL)
	{
		if (tmp->key && ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	find_env_rem(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*current;

	prev = NULL;
	current = *env;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev == NULL)
				*env = current->next;
			else
				prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

void	edit_env(t_env *env, char *key, char *value)
{
	t_env	*tmp;

	tmp = find_env(env, key);
	if (tmp)
	{
		if (tmp->value)
			free(tmp->value);
		if (!value)
			tmp->value = ft_strdup("");
		else
			tmp->value = value;
	}
}

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!env || !new)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*create_env_node(char *key, char *value, int is_exported)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->is_exported = is_exported;
	new->next = NULL;
	return (new);
}
