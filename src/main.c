/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:02:56 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 18:33:39 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mshell.h"

t_mshell	g_mshell;

/*
	@var_dump function is a function that debug double 2d char variable
*/
void	var_dump(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		printf("==>str[%d]:[%s]\n", i, str[i]);
		i++;
	}
}

void	var_dump_token(t_token *tokens)
{
	if (!tokens)
		printf("=====tokens are Null====\n");
	while (tokens)
	{
		printf("==type===(%s)-----\n", tokens->type);
		printf("===value==(%s)====\n", tokens->value);
		tokens = tokens->next;
	}
}

void	var_dump_cmd(t_cmd *cmds)
{
	if (!cmds)
		printf("=====cmds are Null====\n");
	while (cmds)
	{
		printf("===value==(%s)====\n", cmds->arg);
		cmds = cmds->next;
	}
	printf("================end cmds======================\n\n");
}

void	varDumpInFile(t_infile *redirection)
{
	if (!redirection)
	{
		printf("===========NO InFile Exist=====================\n");
		return ;
	}
	printf("===========infile=====================\n");
	while (redirection)
	{
		printf("===========(mode)(%d)(infile)(%s)=====================\n",
			redirection->mode, redirection->filename);
		redirection = redirection->next;
	}
	printf("===========end infile=====================\n");
}

void	varDumpOutFile(t_outfile *redirection)
{
	if (!redirection)
	{
		printf("===========NO outFile=====================\n");
		return ;
	}
	printf("===========outFile=====================\n");
	while (redirection)
	{
		printf("===========(mode)(%d)(outfile)(%s)=====================\n",
			redirection->mode, redirection->filename);
		redirection = redirection->next;
	}
}

//(ls < in | cat -e ) >out  | (grep -c)

void	var_dump_tree(t_tnode *tree)
{
	if (tree)
	{
		// Print node type
		printf("\n|\n|==================>((N_TYPE))(%d)=============>)\n",
			tree->node_type);
		var_dump_cmd(tree->cmd);
		// Check if parent node exists and print its redirection status
		if (tree->t_parent)
		{
			if (tree->t_parent->redirection)
			{
				printf("================= Parent Redirection =================\n");
				varDumpInFile(tree->t_parent->redirection->in_file);
				varDumpOutFile(tree->t_parent->redirection->out_file);
				printf("============= End Parent Redirection =================\n");
			}
			else
				printf("=========== Parent Redirection is NULL ===============\n");
		}
		else
			printf("========== No Parent Node Available ==================\n");
		// Check current node's redirection
		if (tree->redirection)
		{
			if (tree->redirection->in_file)
				varDumpInFile(tree->redirection->in_file);
			else
				printf("========== No In-File Redirection Available ==========\n");
			if (tree->redirection->out_file)
				varDumpOutFile(tree->redirection->out_file);
			else
				printf("========== No Out-File Redirection Available =========\n");
		}
		else
			printf("============ Redirection is NULL =====================\n");
		// Recursively dump the left subtree
		if (tree->t_left)
		{
			printf("\n|_____________> Left Subtree");
			var_dump_tree(tree->t_left);
		}
		else
			printf("\n|----------> Left Subtree is NULL");
		// Recursively dump the right subtree
		if (tree->t_right)
		{
			printf("\n|_____________> Right Subtree");
			var_dump_tree(tree->t_right);
		}
		else
			printf("\n|-----------> Right Subtree is NULL\n");
	}
}

void	var_dump_herdocs(t_herdoc *herdoc)
{
	printf("herdoooooooooooocs==========START====\n");
	while (herdoc)
	{
		printf("------------------id:(%d)--------------------\n", herdoc->id);
		printf("-------------------del:(%s)--------------------\n",
				herdoc->delimiter);
		herdoc = herdoc->next;
	}
	printf("herdoooooooooooocs==========END====\n");
}

/*==================================end of var dump function ===========================*/

void	m_shell_init(char **envp)
{
	g_mshell.cmd_tree = NULL;
	g_mshell.token = NULL;
	g_mshell.pid = get_pid();
	g_mshell.herdocs = NULL;
	g_mshell.n_herdoc = 0;
	g_mshell.flag = 1;
	g_mshell.n_herdoc_executed = 0;
	g_mshell.exit_value = 0;
	extarct_env(envp, &g_mshell.env);
	g_mshell.history = NULL;
	g_mshell.history = (t_history *)malloc(sizeof(t_history));
	g_mshell.history->id = 0;
	g_mshell.history->cmd = NULL;
	g_mshell.history->next = NULL;
}

char	*costum_readline(void)
{
	char	*line;
	char	curr[2024];
	char	*path;
	char	*tmp;

	if (getcwd(curr, 2024) != NULL)
	{
		tmp = ft_strjoin(CYAN "minishell" RESET GREEN "$>" RESET, curr);
		path = ft_strjoin(tmp, ":$ ");
		free(tmp);
	}
	else
		path = ft_strdup(CYAN "minishell" RESET GREEN "$>" RESET ".:$ ");
	if (check_tty())
		line = readline(path);
	else
		line = get_next_line(STDIN_FILENO);
	free(path);
	if (!line)
	{
		if (check_tty())
			ft_putstr_fd("exit\n", 1);
		(free_gvar(1), exit(0));
	}
	return (line);
}

void	ft_handle_history(char **cmd_line)
{
	if (check_tty() && *cmd_line && check_white_spaces(*cmd_line))
	{
		add_history(*cmd_line);
		put_tohistory(*cmd_line, g_mshell.history, 0);
	}
	if (ft_check_comments(*cmd_line))
		ft_handle_comment(cmd_line);
}

void	ft_execute_cli(void)
{
	if (ft_expand_tokens(&g_mshell.token, 1))
	{
		// var_dump_token(g_mshell.token);
		ft_parse_ast(&g_mshell.cmd_tree, &g_mshell.token);
		// var_dump_tree(g_mshell.cmd_tree);
		g_mshell.herdocs = ft_gen_herdocs(g_mshell.token);
		execute(g_mshell.cmd_tree, &g_mshell);
		ft_free_cmd_var();
	}
	else
		ft_free_tokens(&g_mshell.token);
}

int	main(int ac, char **av, char **envp)
{
	char	*cmd_line;

	cmd_line = NULL;
	((void)ac, (void)av);
	m_shell_init(envp);
	while (1)
	{
		handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
		cmd_line = costum_readline();
		ft_handle_history(&cmd_line);
		if (!ft_check_syntax(cmd_line))
		{
			free(cmd_line);
			if (check_tty())
				continue ;
			return (free_gvar(1), EXIT_FAILURE);
		}
		g_mshell.token = ft_tokinizer(cmd_line);
		if (g_mshell.token)
			ft_execute_cli();
		free(cmd_line);
	}
	return (EXIT_SUCCESS);
}
