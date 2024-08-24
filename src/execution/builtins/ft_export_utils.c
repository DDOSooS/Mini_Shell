/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:32:48 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:32:49 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

t_env	*extarct_node(char *args)
{
	t_env	*new;
	char	*key;
	char	*value;
	char	*tmp;

	tmp = ft_strchr(args, '=');
	if (tmp == NULL)
		new = create_env_node(ft_strdup(args), NULL, 0);
	else
	{
		key = ft_substr(args, 0, tmp - args);
		value = ft_strdup(tmp + 1);
		new = create_env_node(key, value, 1);
	}
	return (new);
}

void	appned_export(t_env *env, t_env *new)
{
	t_env	*found_env;
	char	*tmp_value;

	new->key[ft_strlen(new->key) - 1] = '\0';
	found_env = find_env(env, new->key);
	if (found_env == NULL)
	{
		env_add_back(&env, new);
		return ;
	}
	else if (new->value != NULL)
	{
		tmp_value = ft_strjoin(found_env->value, new->value);
		free(found_env->value);
		found_env->value = tmp_value;
		found_env->is_exported = new->is_exported;
		free_env(new);
	}
	else
		free_env(new);
}

void	replacement_export(t_env *env, t_env *new)
{
	t_env	*found_env;

	found_env = find_env(env, new->key);
	if (found_env == NULL)
	{
		env_add_back(&env, new);
		return ;
	}
	else if (new->value != NULL)
	{
		if (found_env->value)
			free(found_env->value);
		found_env->value = ft_strdup(new->value);
		free_env(new);
	}
	else
		free_env(new);
}
