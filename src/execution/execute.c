#include "../../includes/mshell.h"

void cmd_runner(t_cmd *cmd, t_mshell *shell)
{
	ft_printf("this is command\n");
}
void ft_execute_cmd(t_cmd *cmd, t_mshell *shell)
{
	int builtin;

	builtin = -1;
	builtin = builtins_finder(cmd, shell);
	if (builtin == -1)
		cmd_runner(cmd, shell);
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