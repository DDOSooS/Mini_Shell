/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_runner_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:33:55 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:33:56 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static int	count_env_entries(t_env *env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

static char	*create_env_entry(t_env *entry)
{
	char	*tmp_str;
	char	*env_entry;

	if (entry->key)
	{
		tmp_str = ft_strjoin(entry->key, "=");
		if (entry->value)
			env_entry = ft_strjoin(tmp_str, entry->value);
		else
			env_entry = ft_strdup(tmp_str);
		free(tmp_str);
		return (env_entry);
	}
	return (NULL);
}

static void	populate_envp_array(t_env *env, char **envp)
{
	t_env	*tmp;
	int		i;

	tmp = env;
	i = 0;
	while (tmp)
	{
		if (tmp->key)
			envp[i] = create_env_entry(tmp);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
}

char	**get_envp(t_env *env)
{
	char	**envp;
	int		env_count;

	env_count = count_env_entries(env);
	envp = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!envp)
		return (NULL);
	populate_envp_array(env, envp);
	return (envp);
}
