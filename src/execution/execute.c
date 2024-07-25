#include "../../includes/mshell.h"

char *get_last_arg(t_cmd *cmd)
{
	t_cmd *tmp;

	tmp = cmd;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp->arg);
}

void set_under_score(t_env *env, t_cmd *cmd)
{
	t_env *env_under_score;

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

void ft_execute_cmd(t_tnode *root, t_mshell *shell)
{
	t_cmd *cmd;
	int status;

	cmd = root->cmd;
	if (!cmd)
		return;
	status = builtins_finder(cmd, shell, builtins_checker(cmd));
	if (status == -1)
		cmd_runner(cmd, shell);
	else
		shell->exit_value = status;
	set_under_score(shell->env, root->cmd);
}

void reset_in_out(int stdin, int stdout)
{
	if (dup2(stdin, STDIN_FILENO) == -1)
		perror("dup2");
	if (dup2(stdout, STDOUT_FILENO) == -1)
		perror("dup2");
	close(stdin);
	close(stdout);
}

void handle_word(t_tnode *root, t_mshell *shell)
{
	if (apply_redirections(root, shell) == -1)
	{
		shell->exit_value = 1;
		return;
	}
    ft_execute_cmd(root, shell);
}


void ft_execute_parenthises(t_tnode *root, t_mshell *shell)
{
	int pid;

	if (apply_redirections(root, shell) == -1)
	{
		shell->exit_value = 1;
		return;
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
		exit(shell->exit_value);
	}
	else
	{
		waitpid(pid, &shell->exit_value, 0);
		shell->exit_value = get_status(shell->exit_value);
	}
}

void exec_and_or(t_tnode *root, t_mshell *shell)
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

void ft_execute_tree(t_tnode *root, t_mshell *shell)
{
    int (stdout_fd), (stdin_fd);

    stdout_fd = dup(STDOUT_FILENO);
    stdin_fd = dup(STDIN_FILENO);
    if (root == NULL) 
	{
		reset_in_out(stdin_fd, stdout_fd);
        return;
	}
    if (root->node_type == TOKEN_PIPE)
		run_pipe(root, shell);
	else if (root->node_type == TOKEN_PARENTHISE)
		ft_execute_parenthises(root, shell);
	else if (root->node_type == TOKEN_WORD)
		handle_word(root, shell);
	else if (root->node_type == TOKEN_AND
		|| root->node_type == TOKEN_OR)
		exec_and_or(root, shell);
    reset_in_out(stdin_fd, stdout_fd);
}

void execute(t_tnode *root, t_mshell *shell)
{
	if (shell->n_herdoc > 0)
	{
		ft_heredoc(root, shell);
		shell->n_herdoc = 0;
	}
	ft_execute_tree(root, shell);
}
