#include "../../../includes/mshell.h"

static char	*costum_getcwd(void)
{
	char	*buffer;
	char	*tmp;
	size_t	size;

	size = 20;
	buffer = ft_calloc(sizeof(char), size);
	if (buffer != NULL)
		tmp = getcwd(buffer, size);
	else
		return (NULL);
	while (tmp == NULL)
	{
		free(buffer);
		size *= 2;
		buffer = ft_calloc(sizeof(char), size);
		if (buffer != NULL)
			tmp = getcwd(buffer, size);
		else
			return (NULL);
	}
	return (buffer);
}

static void	update_env(char *key, char *value)
{
	t_env	*tmp;

	tmp = find_env(g_mshell.env, key);
	if (tmp != NULL)
	{
		free(tmp->value);
		tmp->value = ft_strdup(value);
	}
	else
		add_env(g_mshell.env, ft_strdup(key), ft_strdup(value));
}

static int	cd_runner(char *dir)
{
	char	*buffer;
	char	*oldpwd;
	char	*curr;

	oldpwd = costum_getcwd();
	if (oldpwd == NULL)
		return (1);
	if (chdir(dir) == 0)
	{
		curr = costum_getcwd();
		update_env("PWD", curr);
		update_env("OLDPWD", oldpwd);
		free(oldpwd);
		free(curr);
		return (0);
	}
	else
	{
		buffer = ft_strjoin("cd: ", dir);
		perror(buffer);
		free(buffer);
	}
	free(oldpwd);
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

	UNUSED(shell);
	cmd_count = count_args(cmd);
	if (cmd_count == 1)
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
