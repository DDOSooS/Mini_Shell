/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:02:56 by aghergho          #+#    #+#             */
/*   Updated: 2024/07/21 05:03:50 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/mshell.h"

t_mshell g_mshell;


/*
	@var_dump function is a function that debug double 2d char variable
*/
void	var_dump(char **str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		ft_printf("==>str[%d]:[%s]\n", i, str[i]);
		i++;
	}
}

void	var_dump_token(t_token *tokens)
{
	// if (! tokens)
	// 	ft_printf("=====tokens are Null====\n");
	while (tokens)
	{
		ft_printf("==type===(%s)-----\n",tokens->type);
		ft_printf("===value==(%s)====\n",tokens->value);
		tokens = tokens->next;
	}
}

void	var_dump_cmd(t_cmd *cmds)
{
	if (! cmds)
		ft_printf("=====cmds are Null====\n");
		
	while (cmds)
	{
		ft_printf("===value==(%s)====\n",cmds->arg);
		cmds = cmds->next;
	}
	ft_printf("================end cmds======================\n\n");
}

void	varDumpInFile(t_infile *redirection)
{
	if (! redirection)
	{
		ft_printf("===========NO InFile Exist=====================\n");
		return ;
	}
	ft_printf("===========infile=====================\n");
	while (redirection)
	{
		ft_printf("===========(mode)(%d)(infile)(%s)=====================\n", redirection->mode, redirection->filename);
		redirection = redirection->next;
	}
	ft_printf("===========end infile=====================\n");
}

void	varDumpOutFile(t_outfile *redirection)
{
	if (! redirection)
	{
		ft_printf("===========NO outFile=====================\n");
		return ;
	}
	ft_printf("===========outFile=====================\n");
	while (redirection)
	{
		ft_printf("===========(mode)(%d)(outfile)(%s)=====================\n", redirection->mode, redirection->filename);
		redirection = redirection->next;
	}		
}

//(ls < in | cat -e ) >out  | (grep -c)

void var_dump_tree(t_tnode *tree)
{
    if (tree)
    {
        // Print node type
        ft_printf("\n|\n|==================>((N_TYPE))(%d)=============>)\n", tree->node_type);
        var_dump_cmd(tree->cmd);
        
        // Check if parent node exists and print its redirection status
        if (tree->t_parent)
        {
            if (tree->t_parent->redirection)
            {
                ft_printf("================= Parent Redirection =================\n");
                varDumpInFile(tree->t_parent->redirection->in_file);
                varDumpOutFile(tree->t_parent->redirection->out_file);
                ft_printf("============= End Parent Redirection =================\n");
            }
            else
                ft_printf("=========== Parent Redirection is NULL ===============\n");
        }
        else
            ft_printf("========== No Parent Node Available ==================\n");

        // Check current node's redirection
        if (tree->redirection)
        {
            if (tree->redirection->in_file)
                varDumpInFile(tree->redirection->in_file);
            else
                ft_printf("========== No In-File Redirection Available ==========\n");
            if (tree->redirection->out_file)
                varDumpOutFile(tree->redirection->out_file);
            else
                ft_printf("========== No Out-File Redirection Available =========\n");
        }
        else
            ft_printf("============ Redirection is NULL =====================\n");
			
        // Recursively dump the left subtree
        if (tree->t_left)
        {
            ft_printf("\n|_____________> Left Subtree");
            var_dump_tree(tree->t_left);
        }
        else
            ft_printf("\n|----------> Left Subtree is NULL");

        // Recursively dump the right subtree
        if (tree->t_right)
        {
            ft_printf("\n|_____________> Right Subtree");
            var_dump_tree(tree->t_right);
        }
        else
            ft_printf("\n|-----------> Right Subtree is NULL\n");
    }
}


void	ft_free_tokens(t_token **tokens)
{
	t_token *tmp;
	
	while (*tokens)
	{
		tmp = (*tokens)->next;
		free((*tokens)->value);	
		free(*tokens);
		*tokens = tmp;
	}
	free(*tokens);
	*tokens = NULL;
}
 
void	ftFreeInFile(t_infile *infile)
{
	t_infile *tmp;

	while (infile->next)
	{
		tmp = infile->next;
		free(infile->filename);
		free(infile);
		infile = tmp;
	}
	free(infile->filename);
	free(infile);
}

void	ftFreeOutFile(t_outfile *outfile)
{
	t_outfile *tmp;

	while (outfile->next)
	{
		tmp = outfile->next;
		free(outfile->filename);
		free(outfile);
		outfile = tmp;
	}
	free(outfile->filename);
	free(outfile);
}

void	ftFreeRedirection(t_redirection *redirection)
{
	if (redirection->in_file)
		ftFreeInFile(redirection->in_file);
	if (redirection->out_file)
		ftFreeOutFile(redirection->out_file);
	free(redirection);
}

void	ft_free_cmds(t_cmd *cmds)
{
	t_cmd *tmp;
	
	while (cmds)
	{
		tmp = cmds->next;
		free(cmds->arg);
		free(cmds);
		cmds = tmp;
	}
	free(cmds);
}

void	ft_free_tree(t_tnode **tree)
{
	if (*tree)
	{
		if (!(*tree)->t_left && !(*tree)->t_right )
		{
			free((*tree)->t_left);
			free((*tree)->t_right);
			ft_free_cmds((*tree)->cmd);
			if ((*tree)->redirection)
				ftFreeRedirection((*tree)->redirection);
			free(*tree);
			*tree = NULL;
			return ;
		}
		else
		{
			ft_free_tree(&(*tree)->t_left);
			ft_free_tree(&(*tree)->t_right);
			free(*tree);
			*tree = NULL;
		}
	}
}

void	var_dump_herdocs(t_herdoc *herdoc)
{
	printf("herdoooooooooooocs==========START====\n");
	while (herdoc)
	{
		printf("------------------id:(%d)--------------------\n", herdoc->id);
		printf("-------------------del:(%s)--------------------\n", herdoc->delimiter);
		herdoc = herdoc->next;
	}
	printf("herdoooooooooooocs==========END====\n");
}

int check_tty()
{
	if (isatty(STDIN_FILENO))
		return (1);
	return (0);
}

t_env *extarct_env(char **envp)
{
	int i;
	t_env *env;
	t_env *tmp;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		tmp = (t_env *)malloc(sizeof(t_env));
		tmp->key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		tmp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		tmp->next = env;
		env = tmp;
		i++;
	}
	//TODO: remove the oldpwd
	// find_env_rem(env, "OLDPWD");
	return (env);
}


void m_shell_init(char **envp)
{
	g_mshell.env = NULL;
	g_mshell.history = NULL;
	g_mshell.env = extarct_env(envp);
	g_mshell.pid = get_pid();
	g_mshell.history = (t_history *)malloc(sizeof(t_history));
	g_mshell.history->id = 0;
	g_mshell.history->cmd = NULL;
	g_mshell.history->next = NULL;
	g_mshell.n_herdoc = 0;
	g_mshell.n_herdoc_executed = 0;
	g_mshell.exit_value = 0;
}

int main(int ac, char **av, char **envp)
{
	//FIXME: edit/work with the t_mshell

	//TODO: Add given file to the programA
	// if (ac >= 2)
	// {
	// 	return (EXIT_FAILURE);
	// }

	char	*cmd_line;
	t_token *tokens;
	t_tnode	*cmd_tree;
	tokens = NULL;
	cmd_tree = NULL;
	ignore_signals();
	/*
		Extract env variables from envp
	*/
	m_shell_init(envp);
	while (1)
	{
		if (check_tty())
			cmd_line = readline("minishell-:>");
		else
			cmd_line = readline("");
		if (!cmd_line)
		{
			if (check_tty())
				ft_printf("exit\n");
			free_gvar();
			return (EXIT_SUCCESS);
		}
		if (!ft_check_syntax(cmd_line))
		{
			free(cmd_line);
			if (check_tty())
				continue;
			else
				return (free(cmd_line), free_gvar(), EXIT_FAILURE);
		}
		if (!ft_strcmp(cmd_line, "exit"))
			return (ft_printf("exit\n"), free(cmd_line), free_gvar(), EXIT_SUCCESS);
		tokens = ft_tokinizer(cmd_line);
		if (tokens)
		{
			// ft_printf("==============first token format===============\n\n");
			var_dump_token(tokens);
			ft_expand_tokens(&tokens);
			var_dump_token(tokens);
			ft_parse_ast(&cmd_tree, &tokens);		
			var_dump_tree(cmd_tree);
			g_mshell.herdocs = ft_gen_herdocs(tokens);
			// var_dump_herdocs(g_mshell.herdocs);
			put_tohistory(cmd_line, g_mshell.history);
			ft_execute_tree(cmd_tree, &g_mshell);
			add_history(cmd_line);
			ft_free_tokens(&tokens);
			ft_free_herdoc(&g_mshell.herdocs);
			// ft_printf("+++==============second token format===============\n\n");
			// var_dump_token(tokens);
			ft_free_tree(&cmd_tree);
		}
		free(cmd_line);
	}
    return (EXIT_SUCCESS);
}