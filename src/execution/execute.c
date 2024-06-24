#include "../../includes/mshell.h"

char **get_path(char *path)
{
	char **path_arr;
	char *tmp;
	int i;

	i = 0;
	if (!path)
		return (NULL);
	tmp = ft_strdup(path);
	path_arr = ft_split(tmp, ':');
	free(tmp);
	return (path_arr);
}
char **cmd_args_getter(t_cmd *cmd)
{
	char **cmd_args;
	t_cmd *tmp;
	int i;

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

void	free_func(char **strings)
{
	int	x;

	x = -1;
	while (strings[++x])
		free(strings[x]);
	free(strings);
}

void run_curr(char **cmd_args, char **paths, char **envp)
{
	if (access(cmd_args[0], F_OK | X_OK | R_OK) == 0 || cmd_args[0][0] == '.')
	{
		if (execve(cmd_args[0], cmd_args, envp) == -1)
		{
			perror("execve");
			free_func(cmd_args);
			if (paths)
				free_func(paths);
			if (envp)
				free_func(envp);
			exit(1);
		}
	}
}

char **get_envp(t_env *env)
{
	char **envp;
	t_env *tmp;
	int i;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	tmp = env;
	while (tmp)
	{
		envp[i] = ft_strjoin(tmp->key, "=");
		envp[i] = ft_strjoin(envp[i], tmp->value);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	*check_command(char *cmd, char **paths)
{
	char	*path_part;
	char	*command;

	command = NULL;
	path_part = NULL;
	if (!paths || !cmd)
		return (NULL);
	while (*paths)
	{
		path_part = ft_strjoin(*paths, "/");
		command = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(command, F_OK | X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

void cmd_runner(t_cmd *cmd, t_mshell *shell)
{
	char **cmd_args;
	char **path;
	char **envp;
	char *cmd_path;
	pid_t pid;

	path = NULL;
	cmd_args = NULL;
	envp = NULL;
	cmd_path = NULL;
	printf("cmd_runner\n");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		cmd_args = cmd_args_getter(cmd);
		if (find_env(shell->env, "PATH"))
			path = get_path(find_env(shell->env, "PATH")->value);
		envp = get_envp(shell->env);
		run_curr(cmd_args, path, envp);
		cmd_path = check_command(cmd_args[0], path);
		if (cmd_path == NULL)
		{
			printf("here (1227)\n");
			perror("command not found");
			free_func(cmd_args);
			if (path)
				free_func(path);
			if (envp)
				free_func(envp);
			exit(127);
		}
		if (execve(cmd_path, cmd_args, envp) == -1)
			(perror(""), exit(1));
	}
	else
	{
		waitpid(pid, &shell->exit_value, 0);
		if (WIFEXITED(shell->exit_value))
			shell->exit_value = WEXITSTATUS(shell->exit_value);
		printf("exit_value: %d\n", shell->exit_value);
	}
	// int i = 0;
	// while (path[i])
	// {
	// 	printf("path[%d]: %s\n", i, path[i]);
	// 	i++;
	// }
	// i = 0;
	// while (cmd_args[i])
	// {
	// 	printf("cmd_args[%d]: %s\n", i, cmd_args[i]);
	// 	i++;
	// }
}

void ft_execute_cmd(t_cmd *cmd, t_mshell *shell)
{
	int builtin;

	builtin = -1;
	builtin = builtins_finder(cmd, shell);
	if (builtin == -1)
	{
		printf("let's run cmd\n");
		cmd_runner(cmd, shell);
	}
}

void ft_execute_tree(t_tnode *root, t_mshell *shell)
{
	if (root)
	{
		if (root->node_type == TOKEN_WORD)
		{
			ft_execute_cmd(root->cmd, shell);
		}
		// else if (root->node_type == TOKEN_PIPE)
		// {
		// 	ft_execute_pipe(root, env);
		// }
		// else if (root->node_type == TOKEN_LOGICAL_OPERATOR)
		// {
		// 	ft_execute_logical_operator(root, env);
		// }
		// else if (root->node_type == TOKEN_IN_REDIRECTION)
		// {
		// 	ft_execute_in_redirection(root, env);
		// }
		// else if (root->node_type == TOKEN_OUT_REDIRECTION)
		// {
		// 	ft_execute_out_redirection(root, env);
		// }
		// else if (root->node_type == TOKEN_L_PARENTHISE)
		// {
		// 	ft_execute_tree(root->t_left, env);
		// }
		// else if (root->node_type == TOKEN_R_PARENTHISE)
		// {
		// 	ft_execute_tree(root->t_right, env);
		// }
	}
}