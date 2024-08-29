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
		if (tmp->value)
			free(tmp->value);
		if (value)
			tmp->value = ft_strdup(value);
		else
			tmp->value = ft_strdup("");
		tmp->is_exported = 1;
	}
}

static int	cd_runner(char *dir)
{
	char	curr[1024];
	char	*oldpwd;

	oldpwd = NULL;
	if (chdir(dir) == 0)
	{
		getcwd(curr, 1024);
		if (find_env(g_mshell.env, "PWD"))
			oldpwd = ft_strdup(find_env(g_mshell.env, "PWD")->value);
		(update_env("PWD", curr), update_env("OLDPWD", oldpwd));
		if (oldpwd)
			free(oldpwd);
		return (0);
	}
	else
	{
		if (errno == 116)
		{
			(write(2, "bash: cd: ", 11), write(2, dir, ft_strlen(dir)));
			write(2, ": No such file or directory\n", 28);
			return (1);
		}
		return (write(2, "bash: cd: ", 11), perror(dir), 1);
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

int	cha_dir_exp(char **dir, t_env *env)
{
	char	*path;
	char	*tmp_char;
	t_env	*tmp;

	tmp = find_env(env, "HOME");
	if (tmp)
		path = ft_strdup(tmp->value);
	else
		path = ft_strdup("/nfs/homes/mkartit");
	if (*dir && (*dir)[0] == '~' && (*dir)[1] == '\0')
	{
		free(*dir);
		*dir = ft_strdup(path);
	}
	else if (*dir && (*dir)[0] == '~' && (*dir)[1] == '/')
	{
		tmp_char = ft_substr(*dir, 1, ft_strlen(*dir) - 1);
		free(*dir);
		*dir = ft_strjoin(path, tmp_char);
		free(tmp_char);
	}
	free(path);
	return (cd_runner(*dir));
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
	else if (cmd_count == 2 && (cmd->next->arg[0] == '~'))
		return (cha_dir_exp(&(cmd->next->arg), shell->env));
	else if (cmd_count == 2)
		return (cd_runner(cmd->next->arg));
	else
	{
		print_stderr("cd: too many arguments");
		return (1);
	}
	return (0);
}
