#include "../../includes/mshell.h"

char	**get_path(char *path)
{
	char	**path_arr;
	char	*tmp;
	// int		i;

	// i = 0;
	if (!path)
		return (NULL);
	tmp = ft_strdup(path);
	path_arr = ft_split(tmp, ':');
	free(tmp);
	return (path_arr);
}

char	**cmd_args_getter(t_cmd *cmd)
{
	char	**cmd_args;
	t_cmd	*tmp;
	int		i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	cmd_args = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	tmp = cmd;
	while (tmp)
	{
		cmd_args[i] = ft_strdup(tmp->arg);
		i++;
		tmp = tmp->next;
	}
	cmd_args[i] = NULL;
	return (cmd_args);
}

void	run_curr(char **cmd_args, char **paths, char **envp)
{
	if (access(cmd_args[0], F_OK | X_OK) == 0 || (cmd_args[0][0] == '.'
		|| cmd_args[0][0] == '/'))
	{
		if (execve(cmd_args[0], cmd_args, envp) == -1)
		{
			ft_printf("minishell: %s: %s\n", cmd_args[0], strerror(errno));
			free_func(cmd_args);
			if (paths)
				free_func(paths);
			if (envp)
				free_func(envp);
			exit(1);
		}
	}
}
int is_directory(char *path)
{
	struct stat sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
		return (1);
	return (0);
}

int other_cases(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "") == 0 || (ft_strlen(cmd) == 2 && cmd[0] == '"'
			&& cmd[1] == '"'))
	{
		write(2, "minishell: '' : command not found\n", 34);
		return (127);
	}
	if (ft_strcmp(cmd, ".") == 0)
	{
		write(2, "minishell: .: filename argument required\n", 41);
		write(2, ".: usage: . filename [arguments]\n", 33);
		return (2);
	}
	if (ft_strcmp(cmd, "..") == 0)
	{
		write(2, "minishell: ..: command not found\n", 33);
		return (127);
	}
	return (0);
}

int check_cmd(char *cmd)
{
	if (cmd && ft_strchr(cmd, '/'))
	{
		if (is_directory(cmd))
		{
			write(2, "minishell: ", 11);
			write(2, cmd, ft_strlen(cmd));
			write(2, ": is a directory\n", 17);
			return (126);
		}
		if (access(cmd, F_OK) == 0)
			return (1);
		else
		{
			write(2, "minishell: ", 11);
			write(2, cmd, ft_strlen(cmd));
			write(2, ": No such file or directory\n", 29);
			return (127);
		}
	}
	return (other_cases(cmd));
}

char	*check_command(char *cmd, char **paths, int *status)
{
	char	*path_part;
	char	*command;

	command = NULL;
	path_part = NULL;
	if (!paths || !cmd)
		return (NULL);
	*status = check_cmd(cmd);
	if (*status == 1)
		return (ft_strdup(cmd));
	else if (*status != 0)
		return (NULL);
	while (*paths)
	{
		path_part = ft_strjoin(*paths, "/");
		command = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(command, F_OK) == 0 && access(command, X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}
