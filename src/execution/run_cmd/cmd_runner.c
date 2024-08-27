/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_runner.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:33:45 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:33:46 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static char	*find_command_path(char **cmd_args, char **path, int *status)
{
	char	*cmd_path;

	cmd_path = NULL;
	cmd_path = check_command(cmd_args[0], path, status);
	if (cmd_path == NULL && *status == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		*status = 127;
	}
	return (cmd_path);
}

static void	execute_command(char **cmd_args, char **path, char **envp)
{
	char	*cmd_path;
	int		status;

	status = 0;
	cmd_path = find_command_path(cmd_args, path, &status);
	if (cmd_path == NULL)
	{
		free_func(cmd_args);
		if (path)
			free_func(path);
		if (envp)
			free_func(envp);
		free_gvar(1);
		exit(status);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		print_file_error(cmd_path, strerror(errno));
		(free(cmd_path), free_func(cmd_args));
		if (path)
			free_func(path);
		if (envp)
			free_func(envp);
		(free_gvar(1), exit(126));
	}
}

static void	handle_child_process(t_cmd *cmd, t_mshell *shell)
{
	char	**cmd_args;
	char	**path;
	char	**envp;

	path = NULL;
	handle_signals(active_sigint, active_sigquit, SIG_IGN, SIG_IGN);
	cmd_args = cmd_args_getter(cmd);
	if (find_env(shell->env, "PATH"))
		path = get_path(find_env(shell->env, "PATH")->value);
	envp = get_envp(shell->env);
	execute_command(cmd_args, path, envp);
}

static void	handle_parent_process(int pid, t_mshell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	unlink_herdoc(shell->herdocs);
	if (status == 2 || status == 131)
	{
		if (status == 2)
			status += 128;
		else
			write(2, "Quit (core dumped)", 18);
		write(2, "\n", 1);
		shell->exit_value = status;
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
