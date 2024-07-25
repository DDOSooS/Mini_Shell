#include "../../includes/mshell.h"

static char	*find_command_path(char **cmd_args, char **path)
{
	char	*cmd_path;

	cmd_path = NULL;
	cmd_path = check_command(cmd_args[0], path);
	if (cmd_path == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	return (cmd_path);
}

static void	execute_command(char **cmd_args, char **path, char **envp)
{
	char	*cmd_path;

	cmd_path = find_command_path(cmd_args, path);
	if (cmd_path == NULL)
	{
		printf("I am here\(1) this command %s\n", cmd_args[0]);
		for (int i = 0; cmd_args[i]; i++)
			printf("cmd_args[%d] = %s\n", i, cmd_args[i]);
		free_func(cmd_args);
		if (path)
			free_func(path);
		if (envp)
			free_func(envp);
		free_gvar();
		exit(127);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		printf("I am here\(2) this command %s\n", cmd_args[0]);
		perror("");
		exit(1);
	}
}

static void	handle_child_process(t_cmd *cmd, t_mshell *shell)
{
	char	**cmd_args;
	char	**path;
	char	**envp;
	int		i;

	path = NULL;
	i = 1;
	handle_signals(active_sigint, active_sigquit, SIG_IGN, SIG_IGN);
	star_expansion(cmd, &cmd_args);
	// cmd_args = cmd_args_getter(cmd);
	if (find_env(shell->env, "PATH"))
		path = get_path(find_env(shell->env, "PATH")->value);
	envp = get_envp(shell->env);
	execute_command(cmd_args, path, envp);
	printf("here(2)\n");
}

static void	handle_parent_process(int pid, t_mshell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (status == 2 || status == 131)
	{
		if (status == 2)
			shell->exit_value = 130;
		else
			write(2, "Quit (core dumped)", 18);
		write(2, "\n", 1);
	}
	else
		shell->exit_value = get_status(status);
}

void	cmd_runner(t_cmd *cmd, t_mshell *shell)
{
	pid_t	pid;

	handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
		handle_child_process(cmd, shell);
	else
		handle_parent_process(pid, shell);
	handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);
}
