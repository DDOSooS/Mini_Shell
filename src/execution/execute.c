/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:56 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/28 22:57:02 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mshell.h"

char	*get_last_arg(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp->arg);
}

void	set_under_score(t_env *env, t_cmd *cmd)
{
	t_env	*env_under_score;

	env_under_score = find_env(env, "_");
	if (env_under_score)
	{
		if (count_args(cmd) > 0)
		{
			free(env_under_score->value);
			env_under_score->value = ft_strdup(get_last_arg(cmd));
		}
		else
		{
			free(env_under_score->value);
			env_under_score->value = ft_strdup("");
		}
	}
}

void	reset_in_out(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}

void	execute(t_tnode *root, t_mshell *shell)
{
	if (shell->n_herdoc > 0 && shell->n_herdoc < 17)
	{
		shell->exit_value = ft_heredoc(root, shell);
		if (shell->exit_value)
			return ;
		shell->n_herdoc = 0;
	}
	ft_execute_tree(root, shell);
}

void	ft_execute_tree(t_tnode *root, t_mshell *shell)
{
	int (stdout_fd), (stdin_fd);
	stdout_fd = dup(STDOUT_FILENO);
	stdin_fd = dup(STDIN_FILENO);
	if (root == NULL)
	{
		reset_in_out(stdin_fd, stdout_fd);
		return ;
	}
	if (root->node_type == TOKEN_PIPE)
		run_pipe(root, shell);
	else if (root->node_type == TOKEN_PARENTHISE)
		ft_execute_parenthises(root, shell);
	else if (root->node_type == TOKEN_AND || root->node_type == TOKEN_OR)
		exec_and_or(root, shell);
	else if (root->node_type == TOKEN_WORD)
		handle_word(root, shell);
	reset_in_out(stdin_fd, stdout_fd);
}
