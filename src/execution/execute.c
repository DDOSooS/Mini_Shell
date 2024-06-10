#include "../../includes/mshell.h"

void ft_execute_cmd(t_cmd *cmd, t_env *env)
{
	char **path;

	path = ft_split(find_env(env, "PATH")->value, ':');

}

void ft_execute_tree(t_tnode *root, t_env *env)
{
	if (root)
	{
		if (root->node_type == TOKEN_WORD)
		{
			ft_execute_cmd(root->cmd, env);
		}
		else if (root->node_type == TOKEN_PIPE)
		{
			ft_execute_pipe(root, env);
		}
		else if (root->node_type == TOKEN_LOGICAL_OPERATOR)
		{
			ft_execute_logical_operator(root, env);
		}
		else if (root->node_type == TOKEN_IN_REDIRECTION)
		{
			ft_execute_in_redirection(root, env);
		}
		else if (root->node_type == TOKEN_OUT_REDIRECTION)
		{
			ft_execute_out_redirection(root, env);
		}
		else if (root->node_type == TOKEN_L_PARENTHISE)
		{
			ft_execute_tree(root->t_left, env);
		}
		else if (root->node_type == TOKEN_R_PARENTHISE)
		{
			ft_execute_tree(root->t_right, env);
		}
	}
}