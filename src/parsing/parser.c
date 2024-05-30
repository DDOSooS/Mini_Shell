/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 20:05:21 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/30 17:49:37 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/mshell.h"

// those function are responsible forn parsing the tokenizer 
// and generate the command table and also well hanele quote expand cases 

/*
    flag is an index to indecate which logical operation 
	flag 0-> it's a commamd to be executed 
    flag 1-> is pipe (|)
    flag 2-> is pipe (||)
    flag 3-> is pipe (&&)

              | 1
            /   \
          (||)2   |1
          /\    / \
        c10  c20  c10 |
                  / \ 
                 0c1  0c2
*/
/*
	flag 0-> it's a commamd to be executed 
    flag is an index to indecate which logical operation 
    flag 1-> is pipe (|)
    flag 2-> is pipe (||)
    flag 3-> is pipe (&&)
*/

int ft_check_operator(t_token *token)
{
	t_token *tmp;
	int flag;

	flag = 0;
	tmp = token;
	while (tmp )
	{
		if (is_l_parenthise(token->value[0]))
			flag++;
		if (is_r_parenthise(token->value[0]))
			flag--;
		if (is_logical_operator(token->value) && !flag)
			return 1;
		tmp = tmp->next;
	}
	return (0);
} 

int ft_check_parenthises(t_token *tokens)
{
	// ft_printf("===============================================\n\n");
	// var_dump_token(tokens);
	// ft_printf("===============================================\n\n");
	while (tokens && tokens->value)
	{
		// ft_printf("===(((((token->value)(%s)==)))))))))<\n", tokens->value);
		if (is_l_parenthise(tokens->value[0]))
			return (1);	
		tokens = tokens->next;
	}
	return (0);
}

int isLastOperator(t_token *tokens)
{
	if (ft_check_and_operator(tokens) || ft_check_or_operator(tokens) )
		return (0);
	return (1);
}

int ft_check_and_operator(t_token *token)
{
    while (token && token->value)
    {
        if (token->value && is_operator(token->value[0]) && isLastOperator(token->next))
		{
			
			// ft_printf("===>>>> yes it's the last logical operator <<<<===\n");
				return (1);
		}
        token = token->next;
    }
    return (0);
}


int ft_check_or_operator(t_token *token)
{
	while (token && token->value)
	{
		// ft_printf("======or_token_verification==(%s)======\n", token->value);
		if (token->value &&  is_pipe(token->value[0]) && is_pipe(token->value[1]) && isLastOperator(token->next))
		{
			// ft_printf("====yes done===\n");
			return (1);
		}
		token = token->next;
	}
	return (0);
}

int ft_check_pipe(t_token *token)
{
	while (token && token->value)
	{
		if (is_pipe(token->value[0]))
		{
				// ft_printf("====yes done===\n");
			return (1);
		}
		token = token->next;
	}
	return (0);
}

int ftCheckWildCard(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '*')
			return (1);
	}
	return (0);
}

/*
	(ls) (-la) (NULL||) cat (NULL&&) (mkdir) (sd) | (ls) || (wc) (makefile) 	
*/
int	ftAddCmd(t_cmd **cmd, char *str)
{
	t_cmd *new;
	t_cmd *tmp;

	new = malloc(sizeof(t_cmd));
	new->arg = str;
	new->check_wildcard = ftCheckWildCard(new->arg);
	new->next = NULL;
	if (!*cmd)
		*cmd = new;
	else
	{
		tmp = *cmd;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
	
}

t_cmd *ftGenCmd(t_token *tokens)
{
	int flag;
	t_cmd	*cmd;

	cmd = NULL;
	flag = 0;
	while (tokens && tokens->value)
	{
		if (!tokens->typeId && !flag)
		{
			if (!ftAddCmd(&cmd, tokens->value))
				return NULL;
		}
		if (tokens->typeId >=6 && tokens->typeId <=9)
			flag = 1;
		if (!tokens->typeId && flag)
			flag = 0;
		tokens = tokens->next;
	}
	return (cmd);
}

int ftAddOutFile(t_outfile **root, t_token *token)
{
	int			mode;
	t_outfile	*new;
	t_outfile	*tmp;

	new = malloc(sizeof(t_outfile));
	if (!new)
		return (0);	
	new->mode =token->typeId;
	new->filename = ft_strdup( token->next->value);
	new->next = NULL;
	// printf("\n===in file=====tmp (%d) -- tmp->next(%s)=====\n", token->typeId, new->filename);
	if (!*root)
	{
		*root = new;
		return (1);	
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	// varDumpOutFile(*root);
	return (1);
}

int ftAddInFile(t_infile **root, t_token *token)
{
	int			mode;
	t_infile	*new;
	t_infile	*tmp;

	new = malloc(sizeof(t_infile));
	if (!new)
		return (0);	
	new->mode = token->typeId;
	if (new->mode == 7)
		new->is_herdoc = 1;
	else
		new->is_herdoc = 0;
	new->filename = ft_strdup (token->next->value);
	new->next = NULL;
	if (!*root)
	{
		*root = new;
		// ft_printf("deoneeeeeeeeeeeeee\n");
		return (1);	
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	// varDumpInFile(*root);
	return (1);
}
// printf("\n===in file=====tmp (%d) -- tmp->next(%s)=====\n", token->typeId, new->filename);


int ftAddRedirection(t_redirection **root, t_token *token)
{
	if (token->typeId == 7 || token->typeId == 8)
	{
		ftAddInFile(&(*root)->in_file, token);
		// ft_printf("========doneeeeeeeeeeeee=========\n");
	}
	if (token->typeId ==6 || token->typeId == 9)
	{
		ftAddOutFile(&(*root)->out_file, token);
		
		// ft_printf("========donee12122121e=========\n");
	}
	return (1);
}

t_redirection *ftGetRedirection(t_token *token)
{
    int             flag;
    t_redirection   *redirection;
    
    // ft_printf("-----HEHO HEHO HEHOOOOOOOOOOOOOOOO-----\n\n");
    // if (token)
    //     ft_printf("-----HEHO HEHO there a token token id == (%s) -----\n\n", token->value);
    
    redirection = malloc(sizeof(t_redirection));
    if (!redirection) {
        perror("malloc");
        return NULL;
    }
    redirection->in_file = NULL;
    redirection->out_file = NULL;
    flag = 0;

    while (token)
    {
        if ((token->typeId >= 1 && token->typeId <= 3) || token->typeId == 5)
            break;
        if (token->typeId >= 6 && token->typeId <= 9)
        {
            ftAddRedirection(&redirection , token);
            token = token->next;
        }
        token = token->next;
    }
    return (redirection);
}

int is_parenthise_redirection(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->typeId == 5)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

t_tnode	*ft_new_tnode(int n_type, t_token *tokens)
{
	t_tnode *new;
 
	// ft_printf("===============>>>>>>>>>ADDDDDD TOOOOKEN<<<<<<<<<=========================\n");
	// var_dump_token(tokens);
	// ft_printf("<><><><><<><><><><><><><><><><><><><><><><>><><><><>><><><><>\n\n\n");
	new = malloc(sizeof(t_tnode));
	if (!new)
		return (NULL);
	new->node_type = n_type;
	new->t_right = NULL;
	new->t_left = NULL;
	new->t_parent = NULL;
	if (n_type)
	{
		new->cmd = NULL;
		if (is_parenthise_redirection(tokens))
		{
			ft_printf("yes>>>>>>>>>>>>>>>>>>>>>>there is<<<<<<<\n");
			while (tokens && tokens->typeId != 5)
				tokens = tokens->next;
			new->redirection = ftGetRedirection(tokens->next);
		}
		else
			new->redirection = NULL;
	}
	else
	{
		// new->cmd = ft_gen_cmd(tokens);
		new->cmd = ftGenCmd(tokens);
		// ft_printf("===============>>>>>>>>>ADDDDDD TOOOOKEN<<<<<<<<<=========================\n");
		// var_dump_token(tokens);
		// ft_printf("<><><><><<><><><><><><><><><><><><><><><><>><><><><>><><><><>\n\n\n");
		// if (! new->cmd)
		// 	return (free(new), NULL);
		// ft_printf("------done command -----\n");
		new->redirection = ftGetRedirection(tokens);
		// ft_printf("------done redirections -----\n");
		// varDumpOutFile(new->redirection->out_file);
		// varDumpInFile(new->redirection->in_file);

		// new->redirection = NULL;
		// if (!new->redirection)
		// 	return(free(new->cmd), free(new), NULL);
	}
	return (new);
}

void ft_parse_parenthise(t_tnode **root, t_token **tokens) {
    t_token *tmp;
    t_tnode *new;
    t_token *tParenthise; // Single pointer
    t_token *tmp1;

    tmp = *tokens;
    while (tmp && tmp->value && !is_l_parenthise(tmp->value[0]))
        tmp = tmp->next;
	free(tmp->value);
	free(tmp->next->next->value);
	tmp->value = NULL;
	tmp->next->next->value = NULL;
	tParenthise = ft_tokinizer(tmp->next->value);
	// if (tParenthise == NULL) 
	// {
	// 	ft_printf("Error: Tokenization failed.\n");
	// 	return;
	// }
	tmp1 = tParenthise;
	while (tmp1->next)
		tmp1 = tmp1->next;
	tmp1->next = tmp->next->next;
	tmp->next = tParenthise;
	ft_parse_ast(root, &tParenthise);
}


void	ft_parse_pipe(t_tnode **root, t_token **tokens)
{
	t_token *tmp;
	t_tnode *new;

	tmp = *tokens;
	// while (tmp && tmp->value && !is_pipe(tmp->value[0]))
	// 	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->value && is_pipe(tmp->value[0]) && !ft_check_pipe(tmp->next))
			break;
		tmp = tmp->next;
	}
	new = ft_new_tnode(1, *tokens);
	// if (!new)
	// 	ft_free_mshell();
	if (!*root)
		*root = new;
	else
	{
		if ((*root)->t_left)
			(*root)->t_right = new;
		else
			(*root)->t_left = new;
	}
	free(tmp->value);
	tmp->value = NULL;
	new->t_parent = *root;
	// ft_printf("=========================================================\n");
	// var_dump_token(tmp->next);
	// ft_printf("==========================================================\n");
	// ft_printf("=======================PIPE ||=============================\n");
	// var_dump_token(*tokens);
	// ft_printf("========================PIPE ||================================\n");
	ft_parse_ast(&new, tokens);
	ft_parse_ast(&new, &tmp->next);	
}

void	ft_parse_or_operator(t_tnode **root, t_token **tokens)
{
	t_token	*tmp;
	t_tnode	*new;

	tmp = *tokens;
	// while (tmp && tmp->value && !(is_pipe(tmp->value[0]) && is_pipe(tmp->value[1])))
	// 	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->value && is_pipe(tmp->value[0]) && is_pipe(tmp->value[1]) && isLastOperator(tmp->next))
			break;
		tmp = tmp->next;
	}
	
	new = ft_new_tnode(2, *tokens);
	// if (!new)
	// 	ft_free_mshell();
	if (!*root)
		*root = new;
	else
	{
		if (!(*root)->t_left)
			(*root)->t_left = new;
		else
			(*root)->t_right = new;
	}
	free(tmp->value);
	tmp->value = NULL;
	// var_dump_token(*tokens);
	// ft_printf("<><><><><><><><><><><>)(())((2st token))(())(<><><><><><><><><><><>\n");
	// var_dump_token(tmp->next);
	// ft_printf("<><><><><><><><><><><>)(())(())(())(<><><><><><><><><><><>\n");
	// ft_printf("====parese or tokens--------(%d)==========\n", new->node_type);
	new->t_parent = *root;
	// ft_printf("=========================================================\n");
	// var_dump_token(tmp->next);
	// ft_printf("==========================================================\n");
	// ft_printf("=======================ORR ||=============================\n");
	// var_dump_token(*tokens);
	// ft_printf("========================OR ||================================\n");
	ft_parse_ast(&new, tokens);
	ft_parse_ast(&new, &(tmp->next));
}

void	ft_parse_and_operator(t_tnode **root, t_token **tokens)
{
	t_token *tmp;
	t_tnode	*new;

	tmp = *tokens;
	// ft_printf("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	// var_dump_token(*tokens);
	// while (tmp && !is_operator(tmp->value[0]))
	// 	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->value && is_operator(tmp->value[0]) && isLastOperator(tmp->next))
			break;
		tmp = tmp->next;
	}
		new = ft_new_tnode(3, *tokens);
	// if (!new)
	// 	ft_free_mshell();
	if (!*root)
		*root = new;
	else
	{
		if (!(*root)->t_left)
			(*root)->t_left = new;
		else
			(*root)->t_right = new;
	}
	free(tmp->value);
	tmp->value = NULL;
	new->t_parent = *root;
	// ft_printf("=======================AND=============================\n");
	// var_dump_token(*tokens);
	// ft_printf("========================AND================================\n");
	ft_parse_ast(&new, tokens);
	ft_parse_ast(&new, &tmp->next);
}

void	ft_parse_cmd(t_tnode **root, t_token **tokens)
{
	t_tnode *new;
	
	// ft_printf("====================cmd_tokens=================\n\n");
	// var_dump_token(*tokens);
	// ft_printf("===============================================\n\n");
	new = ft_new_tnode(0, *tokens);
	// if (!new)
	// 	return (ft_free_mshell());
	// var_dump_token(*tokens);
	if (!*root)
	{
		*root = new;
		return ;
	}
	else
	{
		if 	(!(*root)->t_left)
			(*root)->t_left = new;
		else
			(*root)->t_right = new;
		new->t_parent = *root;
	}
	// ft_printf("=========================================================\n");
	// // var_dump_token(tmp->next);
	// ft_printf("==========================================================\n");

}

void ft_parse_ast(t_tnode **root, t_token **tokens)
{
	t_token *tmp;
	int i;

	i = 0;
	tmp = *tokens;
	while (tmp && tmp->value)
	{
		
		// ft_printf("========================\n\n>>>===(token->value)(%s)==<<<<<<<\n\n", tmp->value);
		
		if (ft_check_and_operator(tmp))
			return (ft_parse_and_operator(root, tokens));
		if (ft_check_or_operator(tmp))
			return (ft_parse_or_operator(root, tokens));
		if (ft_check_pipe(tmp))
			return (ft_parse_pipe(root, tokens));
		if (ft_check_parenthises(tmp))
		{
			return (ft_parse_parenthise(root, tokens));
		}
		return ft_parse_cmd(root, tokens);
		tmp = tmp->next;
	}
}