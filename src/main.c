/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:02:56 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/29 19:58:11 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/mshell.h"



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

        if (tree->redirection)
        {
            if (tree->redirection->in_file)
            {
                // ft_printf("there is an infile=====================\n");
                varDumpInFile(tree->redirection->in_file);
            }
            else
            {
                ft_printf(";((((((  there isn't an infile=====================\n");
            }

            if (tree->redirection->out_file)
            {
                // ft_printf("there is an outfile=====================\n");
                varDumpOutFile(tree->redirection->out_file);
            }
            else
            {
                ft_printf(";((((((  there isn't an outfile=====================\n");
            }
        }
        else
        {
            ft_printf(";((((((  redirection is NULL=====================\n");
        }

        if (tree->t_left)
        {
            ft_printf("\n|_____________>left");
            var_dump_tree(tree->t_left);	
        }
        else
        {
            ft_printf("\n|---------->left NULL");
        }

        if (tree->t_right)
        {
            ft_printf("\n|_____________>right");
            var_dump_tree(tree->t_right);
        }
        else
        {
            ft_printf("\n|----------->right (NULL)\n");
        }
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
 
int main()
{
	char	*cmd_line;
	t_token *tokens;
	t_tnode	*cmd_tree;
	
	cmd_tree = NULL;
	while (1)
	{
			cmd_line = readline("minishell ;)>  ");
			ft_check_syntax(cmd_line);
			tokens = ft_tokinizer(cmd_line);
			ft_printf("==============first token format===============\n\n");
			var_dump_token(tokens);
			ft_parse_ast(&cmd_tree, &tokens);		
			var_dump_tree(cmd_tree);
			if (ft_strcmp(cmd_line, "exit") == 0)
				return (free(cmd_line),0);
			free(cmd_line);
			ft_free_tree(&cmd_tree);
	}
    return (0);
}
