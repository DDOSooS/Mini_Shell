/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 10:30:19 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/24 10:37:04 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	update_shlvl(t_env *env)
{
	t_env	*tmp;
	char	*shlvl;
	int		i;

	tmp = find_env(env, "SHLVL");
	if (tmp)
	{
		i = ft_atoi(tmp->value);
		i++;
		if (i >= 1000)
		{
			write(2, "minishell: warning: shell level (", 33);
			shlvl = ft_itoa(i);
			write(2, shlvl, ft_strlen(shlvl));
			write(2, ") too high, resetting to 1\n", 27);
			i = 1;
			free(shlvl);
		}
		shlvl = ft_itoa(i);
		edit_env(env, "SHLVL", shlvl);
	}
}

t_env	*create_env(void)
{
	t_env	*env;
	char	*path;

	path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	env = NULL;
	env_add_back(&env, create_env_node(ft_strdup("PATH"), ft_strdup(path), 2));
	env_add_back(&env, create_env_node(ft_strdup("PWD"), getcwd(NULL, 0), 1));
	env_add_back(&env, create_env_node(ft_strdup("SHLVL"), ft_strdup("1"), 1));
	env_add_back(&env, create_env_node(ft_strdup("OLDPWD"), NULL, 1));
	env_add_back(&env, create_env_node(ft_strdup("_"),
			ft_strdup("./minishell"), 1));
	return (env);
}

void	extarct_env(char **envp, t_env **env)
{
	int		i;
	char	*sep;
	char	*key;
	char	*value;

	if (envp == NULL || envp[0] == NULL)
	{
		*env = create_env();
		return ;
	}
	i = 0;
	while (envp[i])
	{
		sep = ft_strchr(envp[i], '=');
		if (sep)
		{
			key = ft_substr(envp[i], 0, sep - envp[i]);
			value = ft_strdup(sep + 1);
			env_add_back(env, create_env_node(key, value, 1));
		}
		i++;
	}
	update_shlvl(*env);
}

int	get_status(int status)
{
	return (((status) & 0xff00) >> 8);
}

int	open_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (perror("minishell"), exit(127), 127);
	return (fd);
}
