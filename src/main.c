/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:02:56 by aghergho          #+#    #+#             */
/*   Updated: 2024/07/20 02:33:33 by aghergho         ###   ########.fr       */
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
		// ft_printf("==type===(%s)-----\n",tokens->type);
		// ft_printf("===value==(%s)====\n",tokens->value);
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

void update_shlvl(t_env *env)
{
	t_env *tmp;
	char *shlvl;
	int i;

	tmp = find_env(env, "SHLVL");
	if (tmp)
	{
		i = ft_atoi(tmp->value);
		i++;
		shlvl = ft_itoa(i);
		edit_env(env, "SHLVL", shlvl);
		// free(shlvl);
	}
}

t_env *create_env(void)
{
	t_env *env;
	char *path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";

	env = NULL;
	env_add_back(&env, create_env_node(ft_strdup("PATH"), ft_strdup(path), 2));
	env_add_back(&env, create_env_node(ft_strdup("PWD"), getcwd(NULL, 0), 1));
	env_add_back(&env, create_env_node(ft_strdup("SHLVL"), ft_strdup("1"), 1));
	env_add_back(&env, create_env_node(ft_strdup("OLDPWD"), NULL, 2));
	return (env);
}

void extarct_env(char **envp, t_env **env)
{
	int i;
	char *sep;
	char *Key;
	char *value;

	if (envp == NULL || envp[0] == NULL)
	{
		*env = create_env();
		return ;
	}
	i = 0;
	while (envp[i])
	{
		sep = ft_strchr(envp[i], '=');
		if (sep)
		{
			Key = ft_substr(envp[i], 0, sep - envp[i]);
			value = ft_strdup(sep + 1);
			env_add_back(env, create_env_node(Key, value, 1));
		}
		i++;
	}
	find_env_rem(*env, "_");
	update_shlvl(*env);
}

void m_shell_init(char **envp)
{
	g_mshell.pid = get_pid();
	extarct_env(envp, &g_mshell.env);
	g_mshell.history = NULL;
	g_mshell.history = (t_history *)malloc(sizeof(t_history));
	g_mshell.history->id = 0;
	g_mshell.history->cmd = NULL;
	g_mshell.history->next = NULL;
	g_mshell.n_herdoc = 0;
	g_mshell.n_herdoc_executed = 0;
	g_mshell.exit_value = 0;
}

char *costum_readline()
{
	char *line;

	if (check_tty())
		line = readline("minishell:$ ");
	else
		line = get_next_line(STDIN_FILENO);
	if (!line)
	{
		if (check_tty())
			ft_printf("exit\n");
		free_gvar();
		exit(0);
	}
	return (line);
}
int is_white_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int check_white_spaces(char *cmd)
{
	int i;

	i = 0;
	while (cmd[i])
	{
		if (is_white_space(cmd[i]) != 1)
			return (1);
		i++;
	}
	return (0);
}

int	get_status(int status)
{
	return (((status) & 0xff00) >> 8);
}

int main(int ac, char **av, char **envp)
{
	char	*cmd_line;
	t_token *tokens;
	t_tnode	*cmd_tree;
	tokens = NULL;
	cmd_tree = NULL;
	m_shell_init(envp);
	//TODO: add the signals handler for (ctrl + c) and (ctrl + \)
	//TODO: and for every the child process
	while (1)
	{
		cmd_line = costum_readline();
		if (check_tty() && *cmd_line && check_white_spaces(cmd_line))
			add_history(cmd_line);
		put_tohistory(cmd_line, g_mshell.history);
		if (!ft_check_syntax(cmd_line))
		{
			free(cmd_line);
			if (check_tty())
				continue;
			return (free(cmd_line), free_gvar(), EXIT_FAILURE);
		}
		if ((tokens = ft_tokinizer(cmd_line)) != NULL)
		{
			// ft_printf("==============first token format===============\n\n");
			// var_dump_token(tokens);
			ft_expand_tokens(&tokens);
			ft_parse_ast(&cmd_tree, &tokens);		
			// var_dump_tree(cmd_tree);
			g_mshell.herdocs = ft_gen_herdocs(tokens);
			// var_dump_token(tokens);
			// var_dump_herdocs(g_mshell.herdocs);
			execute(cmd_tree, &g_mshell);
			ft_free_tokens(&tokens);
			ft_free_herdoc(&g_mshell.herdocs);
			// ft_printf("+++==============second token format===============\n\n");
			// var_dump_token(tokens);
			ft_free_tree(&cmd_tree);
			// printf("exit_value: %d\n", g_mshell.exit_value);
		}
		free(cmd_line);
	}
    return (EXIT_SUCCESS);
}
