/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:02:56 by aghergho          #+#    #+#             */
/*   Updated: 2024/06/10 13:40:06 by aghergho         ###   ########.fr       */
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
	if (! tokens)
		ft_printf("=====tokens are Null====\n");
	while (tokens)
	{
		ft_printf("==type===(%s)-----\n",tokens->type);
		ft_printf("===value==(%s)====\n",tokens->value);
		tokens = tokens->next;
	}
}

void	var_dump_cmd(t_cmd *cmds)
{
	// ft_printf("================cmds==(%d)====================\n\n", cmds->arg);
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
	ft_printf("===========infile=====================\n\n");
	while (redirection)
	{
		ft_printf("===========(mode)(%d)(infile)(%s)=====================\n", redirection->mode, redirection->filename);
		redirection = redirection->next;
	}
}

void	varDumpOutFile(t_outfile *redirection)
{
	ft_printf("===========outFile=====================\n\n");
	while (redirection)
	{
		ft_printf("===========(mode)(%d)(outfile)(%s)=====================\n", redirection->mode, redirection->filename);
		redirection = redirection->next;
	}		
}


void var_dump_tree(t_tnode *tree)
{
    if (tree)
    {
        ft_printf("\n|\n|==================>((N_TYPE))(%d)=============>)\n", tree->node_type);
        var_dump_cmd(tree->cmd);
		if (tree->t_parent)
		{
			if (tree->t_parent->redirection)
			{
				ft_printf("=================parent redirection ()*)*)*)*)(*)*)*)=======\n\n");
				varDumpInFile(tree->t_parent->redirection->in_file);
				varDumpOutFile(tree->t_parent->redirection->out_file);
				ft_printf("==========END=======parent redirection ()*)*)*)*)(*)*)*)=======\n\n");
			}
			else
				ft_printf("=================parent redirection NUUUUUUUUUULLLLLLLLLLLLLLLLLLL ()*)*)*)*)(*)*)*)=======\n\n");
				
		}
		else
			ft_printf("there's no parent=======[<><><><><><<><>><<<<<<<><><><><<>><><><><><><>]\n");
		
        if (tree->redirection)
        {
            if (tree->redirection->in_file)
                varDumpInFile(tree->redirection->in_file);
            else
                ft_printf(";((((((  there isn't an infile=====================\n");
            if (tree->redirection->out_file)
                varDumpOutFile(tree->redirection->out_file);
            else
                ft_printf(";((((((  there isn't an outfile=====================\n");
        }
        else
            ft_printf(";((((((  redirection is NULL=====================\n");
        if (tree->t_left)
        {
            ft_printf("\n|_____________>left");
            var_dump_tree(tree->t_left);	
        }
        else
            ft_printf("\n|---------->left NULL");

        if (tree->t_right)
        {
            ft_printf("\n|_____________>right");
            var_dump_tree(tree->t_right);
        }
        else
            ft_printf("\n|----------->right (NULL)\n");
    }
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
void	ft_free_tree(t_tnode **tree)
{
	if (*tree)
	{
		if (!(*tree)->t_left && !(*tree)->t_right )
		{
			free((*tree)->cmd);
			free((*tree)->t_left);
			free((*tree)->t_right);
			if ((*tree)->redirection)
				ftFreeRedirection((*tree)->redirection);
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
 
int main(int ac, char **av, char **env)
{
	char	*cmd_line;
	// t_env	*env_list;
	t_token *tokens;
	t_tnode	*cmd_tree;
	cmd_tree = NULL;
	/*
		cntl + c signal handler - new prompt in and new line
		cntl + \ signal handler - do nothing
	*/
	ignore_signals();
	/*
		Extract env variables from envp
	*/
	g_mshell.env = extarct_env(env);
	g_mshell.pid= get_pid();
	g_mshell.n_herdoc = 0;
	// env_list = g_mshell.env;
	// while (env_list)
	// {
	// 	ft_printf("key: %s value:%s\n", env_list->key, env_list->value);
	// 	env_list = env_list->next;
	// }
	ft_printf("====get_pid(%d) === myowne_pid(%d)===\n", getpid(), get_pid());
	while (1)
	{
			cmd_line = readline("minishell ;)>  ");
			if (cmd_line == NULL)
				return (0);
			ft_check_syntax(cmd_line);
			tokens = ft_tokinizer(cmd_line);
			ft_printf("==============first token format===============\n\n");
			var_dump_token(tokens);
			ft_expand_tokens(tokens);
			var_dump_token(tokens);
			ft_parse_ast(&cmd_tree, &tokens);		
			var_dump_tree(cmd_tree);
			// ft_execute_tree(cmd_tree, env);
			// add_history(cmd_line);
			if (ft_strcmp(cmd_line, "exit") == 0)
				return (free(cmd_line),0);
			free(cmd_line);
			ft_free_tree(&cmd_tree);
	}
    return (EXIT_SUCCESS);
}
