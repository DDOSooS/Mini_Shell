/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cases.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:33:03 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/28 18:13:34 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	exec_and_or(t_tnode *root, t_mshell *shell)
{
	if (root->node_type == TOKEN_AND)
	{
		ft_execute_tree(root->t_left, shell);
		if (shell->exit_value == 0)
			ft_execute_tree(root->t_right, shell);
	}
	else if (root->node_type == TOKEN_OR)
	{
		ft_execute_tree(root->t_left, shell);
		if (shell->exit_value != 0)
			ft_execute_tree(root->t_right, shell);
	}
}

void	ft_execute_parenthises(t_tnode *root, t_mshell *shell)
{
	int	pid;

	if (apply_redirections(root) == -1)
	{
		shell->exit_value = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		ft_execute_tree(root->t_left, shell);
		free_gvar(1);
		exit(shell->exit_value);
	}
	else
	{
		waitpid(pid, &shell->exit_value, 0);
		shell->exit_value = get_status(shell->exit_value);
	}
}

void	ft_execute_cmd(t_tnode *root, t_mshell *shell)
{
	t_cmd	*cmd;
	int		status;

	cmd = NULL;
	status = 0;
	cmd = root->cmd;
	if (!cmd)
		return ;
	status = builtins_finder(cmd, shell, builtins_checker(cmd));
	if (status == -1)
		cmd_runner(cmd, shell);
	else
		shell->exit_value = status;
	set_under_score(shell->env, root->cmd);
}

void	handle_word(t_tnode *root, t_mshell *shell)
{
	if (apply_redirections(root) == -1)
	{
		shell->exit_value = 1;
		return ;
	}
	ft_execute_cmd(root, shell);
}
