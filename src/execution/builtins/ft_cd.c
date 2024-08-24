/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:32:12 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/23 10:18:44 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static void	update_env(char *key, char *value)
{
	t_env	*tmp;

	tmp = find_env(g_mshell.env, key);
	if (tmp != NULL)
	{
		if (value)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
		}
		tmp->is_exported = 1;
	}
}

static int	cd_runner(char *dir)
{
	char	oldpwd[1024];
	char	curr[1024];

	getcwd(oldpwd, 1024);
	if (chdir(dir) == 0)
	{
		getcwd(curr, 1024);
		update_env("PWD", curr);
		update_env("OLDPWD", oldpwd);
		return (0);
	}
	else
	{
		if (errno == 116)
		{
			write(2, "bash: cd: ", ft_strlen("bash: cd: "));
			write(2, dir, ft_strlen(dir));
			write(2, ": No such file or directory\n", 28);
			return (1);
		}
		write(2, "bash: cd: ", ft_strlen("bash: cd: "));
		return (perror(dir), 1);
	}
	return (1);
}

static int	cha_dir(char *dir)
{
	char	*path;
	char	*buffer;
	char	*tmp;

	tmp = NULL;
	buffer = NULL;
	path = NULL;
	if (find_env(g_mshell.env, dir) != NULL)
		path = find_env(g_mshell.env, dir)->value;
	if (path == NULL || path[0] == '\0')
	{
		tmp = ft_strjoin("cd: ", dir);
		if (tmp)
		{
			buffer = ft_strjoin(tmp, " not set\n");
			if (buffer)
				write(2, buffer, ft_strlen(buffer));
			free(buffer);
		}
		free(tmp);
		return (1);
	}
	return (cd_runner(path));
}

int	ft_cd(t_cmd *cmd, t_mshell *shell)
{
	int	cmd_count;

	(void)(shell);
	cmd_count = count_args(cmd);
	if (cmd->next && cmd->next->arg && ft_strcmp(cmd->next->arg, "") == 0)
		return (1);
	else if (cmd_count == 1)
		return (cha_dir("HOME"));
	else if (cmd_count == 2 && (ft_strcmp(cmd->next->arg, "-") == 0))
		return (cha_dir("OLDPWD"));
	else if (cmd_count == 2)
		return (cd_runner(cmd->next->arg));
	else
	{
		print_stderr("cd: too many arguments");
		return (1);
	}
	return (0);
}
