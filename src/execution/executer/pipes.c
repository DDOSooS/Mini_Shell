/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:33:24 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:33:25 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

static int	fork_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	setup_left_child(int pipefd[2], t_tnode *root, t_mshell *shell)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	ft_execute_tree(root->t_left, shell);
	free_gvar(1);
	exit(shell->exit_value);
}

void	setup_right_child(int pipefd[2], t_tnode *root, t_mshell *shell)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	ft_execute_tree(root->t_right, shell);
	free_gvar(1);
	exit(shell->exit_value);
}

void	run_pipe(t_tnode *root, t_mshell *shell)
{
	int	pipefd[2];

	int (pid_left), (pid_right), (status);
	create_pipe(pipefd);
	pid_left = fork_process();
	if (pid_left == 0)
		setup_left_child(pipefd, root, shell);
	pid_right = fork_process();
	if (pid_right == 0)
		setup_right_child(pipefd, root, shell);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	shell->exit_value = get_status(status);
}
