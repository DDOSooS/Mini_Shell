/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/07/19 03:25:37 by aghergho         ###   ########.fr       */
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
	while (tokens && tokens->value)
	{
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
				return (1);
        token = token->next;
    }
    return (0);
}

int ft_check_or_operator(t_token *token)
{
	while (token && token->value)
	{
		if (token->value &&  is_pipe(token->value[0]) && is_pipe(token->value[1]) && isLastOperator(token->next))
			return (1);
		token = token->next;
	}
	return (0);
}

int ft_check_pipe(t_token *token)
{
	while (token && token->value)
	{
		if (is_pipe(token->value[0]))
			return (1);
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

int	ftAddCmd(t_cmd **cmd, char *str)
{
	t_cmd *new;
	t_cmd *tmp;

	new = malloc(sizeof(t_cmd));
	new->arg = ft_strdup(str);
	new->check_wildcard = ftCheckWildCard(new->arg);
	new->next = NULL;
	if (! cmd || !*cmd)
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

int ft_add_to_cmd (t_cmd **root, char *token)
{
	char *cmd;
	t_cmd	*tmp;
	t_cmd	*new;
	char	*tmp_token;

	tmp = *root;
	cmd = ft_substr(token, 1, ft_strlen(token) - 2);
	free(token);
	new = malloc(sizeof(t_cmd));
	new->arg = cmd;
	new->check_wildcard = 0;
	new->next = NULL;
	if (!root || !*root)
	{
		*root = new;
		return (1);
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
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
			if (tokens->value[0] && is_double_quote(tokens->value[0]) && is_double_quote(tokens->value[ft_strlen(tokens->value) -1]))
				ft_add_to_cmd(&cmd, tokens->value);
			else if (!ftAddCmd(&cmd, tokens->value))
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
	if (!*root)
	{
		*root = new;
		return (1);	
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
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
		return (1);	
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (1);
}

int ftAddRedirection(t_redirection **root, t_token *token, int inredirection , int out_redirection)
{
	if ((token->typeId == 7 || token->typeId == 8) && inredirection)
		ftAddInFile(&(*root)->in_file, token);
	if ((token->typeId ==6 || token->typeId == 9) && out_redirection)
		ftAddOutFile(&(*root)->out_file, token);
	return (1);
}

void ftGetRedirection(t_redirection **redirection, t_token *token, int inflag, int outflag)
{
	//TO DO CHECK LEAKS
    int             flag;

	if (!*redirection)
	{
    	*redirection = malloc(sizeof(t_redirection));
    	(*redirection)->in_file = NULL;
    	(*redirection)->out_file = NULL;
	}
    flag = 0;
    while (token)
    {
        if ((token->typeId >= 1 && token->typeId <= 3) || token->typeId == 5)
            break;
        if (token->typeId >= 6 && token->typeId <= 9)
        {
            ftAddRedirection(redirection , token, inflag, outflag);
            token = token->next;
        }
        token = token->next;
    }

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

int check_unclosed_quote(char *token)
{
	int status;
	int	i;

	i = -1;
	status = 1;
	while (token[++i] && ft_check_quote(token, i + 1));
	if (token[i] && ft_check_quote(token,i))
		status = 0;
	return (status);
}

// ==============================count the expanded args's lesn ====================================

int ft_check_env_var(char *str) 
{
	t_env *tmp;

	tmp = g_mshell.env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, str))
			return (ft_strlen(tmp->value));
		tmp = tmp->next;
	}
	return (0);
}

int ft_count_number_len(char token)
{
	int counter;
	int number;
	
	if (token == '?')
		number = g_mshell.exit_value;
	else
		number = g_mshell.pid;
	counter = 0;
	while (number > 9)
	{	
		number = number / 10;
		counter++;
	}
	counter++;
	return (counter);
}

int ft_search_expanded_token(char *token, int *counter)
{
	int i;
	int start;
	char *str;
	int len;
	
	i = 0;
	start = 1;
	while (token[++i] && !is_quote(token[i + 1]) && !is_whites_space(token[i + 1]) && !ft_check_quote(token, i + 1));
	str = ft_substr(token, 1, i);
	start = ft_check_env_var(str);
	(*counter) = (*counter) + start;
	free(str);
	return (i);
}

int ft_get_unexpanded_token(char *token, int *counter)
{
	int i;
	
	i = 0 ;
	if (check_unclosed_quote(token))
		(*counter)++;
	while (token[++i] && ft_check_quote(token, i + 1))
		(*counter)++;
	return (i)	;
}


int ft_get_expanded_quoted_token(char *token, int *counter)
{
	int start;
	int i;

	i = 0;
	while (token[++i] && ft_check_quote(token, i + 1))
	{
		if (!is_dollar_sign(token[i]))
		{
			(*counter)++;
			continue;
		}
		if (is_dollar_sign(token[i]) && token[i + 1] && is_dollar_sign(token[i + 1]))
		{
			(*counter) += ft_count_number_len(token[i+1]);
			i++;
		}
		else if (is_dollar_sign(token[i]) && token[i + 1] && !is_whites_space(token[i + 1]) && ! is_double_quote(token[i + 1]))
			i += ft_search_expanded_token(&token[i], counter);
		else
		{
			(*counter)++;
		}
	}
	return (i);
}

int ft_get_expanded_unquoted_token(char *token, int *counter)
{
	int		i;
	char	*str;
	
	i = 0;


	if (token[i + 1] && is_dollar_sign(token[i]) && ft_isdigit(token[i + 1]))
		return (1);
	if (token[i + 1] && (is_dollar_sign(token[i + 1]) || token[i + 1] == '?'))
	{
		(*counter) += ft_count_number_len(token[i + 1]);
		return (1);		
	}
	else if ( is_dollar_sign(token[i]) && (!token[i + 1] || is_quote(token[i + 1]) || is_whites_space(token[i + 1])))
	{
		(*counter)++;
		return (0);
	}
	while(token[++i] && ft_isalnum(token[i + 1]) && !is_single_quote(token[i + 1]) && !is_whites_space(token[i + 1]) && !is_dollar_sign(token[i+1]));
	str = ft_substr(token, 1, i);
	(*counter) += ft_check_env_var(str);
	free(str);
	return (i);
}


int ft_count_expanded_token(char *token, int *counter)
{
	int i;

	i = -1;
	while (token[++i])
	{
		if (is_single_quote(token[i]))
		{
			i += ft_get_unexpanded_token(&token[i ], counter);
			return (i);	
		}
		else if (is_quote(token[i]))
		{
			i += ft_get_expanded_quoted_token(&token[i], counter) ;
			return (i);
		}
		else if (is_dollar_sign(token[i]))
		{
			i += ft_get_expanded_unquoted_token(&token[i], counter);
			return (i);			
		}
	}
	return (i);
}

int	ft_expanded_token(char *token)
{
	int i;
	int	counter;

	counter = 0;
	i = -1;
	while (token[++i])
	{
		if (is_quote(token[i]) || is_dollar_sign(token[i]))
			i += ft_count_expanded_token(&token[i], &counter);		
		else
			counter++;
	}
	return (counter);
}

// =========================================================generate the expanded args ==================================================================

int ft_gen_pid_token(char **str, char token)
{
	char *tmp;
	int number;
	
	if (token == '?')
		number = g_mshell.exit_value;
	else
		number = g_mshell.pid;	
	tmp = ft_itoa(number);
	if (!tmp)
		return (0);
	*str = ft_strcat(*str, tmp);
	free(tmp);
	return (1);
}


char *ft_get_env_var(char *str)
{
	t_env *tmp;

	tmp = g_mshell.env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, str))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int ft_gen_expanded_unquoted_token(char **s1, char *token)
{
	int		i;
	char	*str;
	int		env_len;
	char	*tmp;
	
	i = 0;
	if (token[i+ 1] && is_dollar_sign(token[i]) && ft_isdigit(token[i + 1]))
		return (1);
	if (token[i + 1] && (is_dollar_sign(token[i + 1]) || token[i + 1] == '?'))
	{
		ft_gen_pid_token(s1, token[i+1]);
		return (1);		
	}
	else if ( is_dollar_sign(token[i]) && (!token[i + 1] || is_quote(token[i + 1]) || is_whites_space(token[i + 1])))
	{
		*s1 = ft_strcat_char(*s1, token[i]);
		return (0);
	}
	while(token[++i] && ft_isalnum(token[i + 1]) && !is_single_quote(token[i + 1]) && !is_whites_space(token[i + 1]) && !is_dollar_sign(token[i+1]));
	str = ft_substr(token, 1, i);
	env_len = ft_check_env_var(str);
	if (env_len)
	{
		tmp = ft_get_env_var(str);
		*s1 = ft_strcat(*s1, tmp);
	}
	free(str);
	return (i);
}


int ft_gen_search_expanded_token(char **s1, char *token)
{
	int i;
	char *str;
	int	env_len;
	char *tmp;
		
	i = 0;
	while (token[++i] && !is_quote(token[i + 1]) && !is_whites_space(token[i + 1]) && !ft_check_quote(token, i + 1));
	str = ft_substr(token, 1, i);
	env_len = ft_check_env_var(str);
	if (env_len)
	{
		tmp = ft_get_env_var(str);
		*s1 = ft_strcat(*s1, tmp);
	}
	free(str);
	return (i);
}

int ft_gen_expanded_quoted_token(char **str, char *token)
{
	int i;

	i = 0;
	while (token[++i] && ft_check_quote(token, i + 1))
	{
		if (!is_dollar_sign(token[i]))
		{
			*str = ft_strcat_char(*str, token[i]);
			continue;
		}
		if (is_dollar_sign(token[i]) && token[i + 1] && is_dollar_sign(token[i + 1]))
		{
			ft_gen_pid_token(str, token[i+1]);
			i++;
		}
		else if (is_dollar_sign(token[i]) && token[i + 1] && !is_whites_space(token[i + 1]) && ! is_double_quote(token[i + 1]))
			i += ft_gen_search_expanded_token(str,&token[i]);
		else
			*str = ft_strcat_char(*str, token[i]);
	}
	return (i);
}

int ft_gen_unexpanded_token(char **str, char *token)
{
	int i;
	
	i = 0 ;
	if (check_unclosed_quote(token))
		*str = ft_strcat_char(*str, token[i]);
	while (token[++i] && ft_check_quote(token, i + 1))
		*str = ft_strcat_char(*str, token[i]);
	return (i)	;
}

int ft_gen_expanded_token(char **str,char *token)
{
	int i;

	i = -1;
	while (token[++i])
	{
		if (is_single_quote(token[i]))
		{
			i += ft_gen_unexpanded_token(str, &token[i]);
			return (i);			
		}
		else if (is_double_quote(token[i]))
		{
			i += ft_gen_expanded_quoted_token(str, &token[i]) ;
			return (i);
		}
		else if (is_dollar_sign(token[i]))
		{
			i += ft_gen_expanded_unquoted_token(str, &token[i]);
			return (i);
		}
	}
	return (i);
}


int	ft_gen_expanded_arg(char **str, char *token)
{
	int i;
	int	counter;

	counter = 0;
	i = -1;
	while (token[++i])
	{
		if (is_quote(token[i]) || is_dollar_sign(token[i]))
			i += ft_gen_expanded_token(str, &token[i]);		
		else
		{
			counter++;
			*str = ft_strcat_char(*str , token[i]);
		}
	}
	return (counter);
}

/*
	this function is responsible for expanding any envp Var
*/

int ft_expand_arg(char **arg)
{
	int len;
	char *tmp;
	char *new;

	tmp = *arg;
	len = ft_expanded_token(tmp);
	new = malloc(sizeof(char) * (len +  1));
	if (!new)
		return (0);
	new[0] = '\0';
	ft_gen_expanded_arg(&new, tmp);
	free(*arg);
	*arg = new;
	return (1);
}

void vard_dmum_char(char **args)
{
	int i;

	i = 0;
	while (args[i])
	{
		printf ("++++++++=args[%d]=>(%s)<<<<<<<\n", i,args[i]);
		i++;
	}
}

t_cmd *ft_gen_new_cmds(char *arg)
{
	t_cmd *new;
	char **cmds;
	int	i;

	i = 0;
	new = NULL;
	cmds = ft_split_words(arg, " \t");
	while (cmds[i])
	{
		ftAddCmd(&new, cmds[i]);
		free(cmds[i]);
		i++;
	}
	free(cmds);
	return (new);
}

t_cmd  *ft_split_cmd( char *arg)
{
	t_cmd	*new;
	t_cmd	*s_tmp;
	
	new = ft_gen_new_cmds(arg);
	free(arg);
	return (new);
}

void ft_expand_cmd(t_token **root)
{
	t_token		*tmp;
	
	tmp = *root;
	while (tmp)
	{
		if (ft_check_expand(tmp->value))
			ft_expand_arg(&tmp->value);
		tmp = tmp->next;
	}
}

//++++++++++++++++++++++++++++++++expand of out files+++++++++++++++++++++++++++++++++++++++++++++++++


int ft_get_expanded_quoted_delimiter(char *token, int *counter)
{
	int i;

	i = 0;
	while (token[++i] && ft_check_quote(token, i + 1))
	{
		(*counter)++;
	}
	return (i);
}
	

int ft_get_unexpanded_delimiter(char *token, int *counter)
{
	int i;
	
	i = 0 ;
	while (token[++i] && ft_check_quote(token, i + 1))
		(*counter)++;
	return (i)	;
}

int ft_count_expanded_deliter(char *token, int *counter)
{
	int i;

	i = -1;
	while (token[++i])
	{
		if (is_single_quote(token[i]))
		{
			i += ft_get_unexpanded_delimiter(&token[i ], counter);
			return (i);	
		}
		else if (is_quote(token[i]))
		{
			i += ft_get_expanded_quoted_delimiter(&token[i], counter) ;
			return (i);
		}
	}
	return (i);
}


int	ft_count_delimter_len(char *token)
{
	int i;
	int	counter;

	counter = 0;
	i = -1;
	while (token[++i])
	{
		if (is_quote(token[i]))
			i += ft_count_expanded_deliter(&token[i], &counter);		
		else
			counter++;
	}
	return (counter);
}

//________________________________________Generating the delimiter__________________

int ft_gen_expanded_quoted_delimiter(char **str ,char *token)
{
	int i;

	i = 0;
	while (token[++i] && ft_check_quote(token, i + 1))
		*str = ft_strcat_char(*str, token[i]);
	return (i);
}


int ft_gen_unexpanded_delimiter(char **str,char *token)
{
	int i;
	
	i = 0 ;
	while (token[++i] && ft_check_quote(token, i + 1))
		*str = ft_strcat_char(*str, token[i]);
	return (i)	;
}

int ft_gen_expanded_deliter(char **str, char *token)
{
	int i;

	i = -1;
	while (token[++i])
	{
		if (is_single_quote(token[i]))
		{
			i += ft_gen_unexpanded_delimiter(str, &token[i ]);
			return (i);
		}
		else if (is_quote(token[i]))
		{
			i += ft_gen_expanded_quoted_delimiter(str, &token[i]) ;
			return (i);
		}
	}
	return (i);
}

int	ft_gen_delimter(char **str,char *token)
{
	int i;

	i = -1;
	while (token[++i])
	{
		if (is_quote(token[i]))
			i += ft_gen_expanded_deliter(str ,&token[i]);		
		else
			*str = ft_strcat_char(*str, token[i]);
	}
	return (1);
}

void ft_expand_delimiter(char **arg)
{
	// int len;
	// char *tmp;
	// char *new;
	// tmp = *arg;
	// len = ft_count_delimter_len(tmp);
	// new = malloc(sizeof(char) * (len +  1));
	// new[0] = '\0';
	// ft_gen_delimter(&new, tmp);
	// free (tmp);
	// *arg = new;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ft_expand_infile(t_infile **root)
{
	t_infile *tmp;

	tmp = *root;
	while (tmp)
	{
		if (! tmp->is_herdoc)
			ft_expand_arg(&tmp->filename);
		// if (tmp->is_herdoc)
		// 	ft_expand_delimiter(&tmp->filename);
		// else
		// 	ft_expand_arg(&tmp->filename);
		tmp = tmp->next;
	}
}

void ft_expand_outfile(t_outfile **root)
{
	t_outfile *tmp;

	tmp = *root;
	while (tmp)
	{
		ft_expand_arg(&tmp->filename);
		
		tmp = tmp->next;
	}
}

void ft_expand_redirection(t_redirection **redirection)
{
	ft_expand_infile(&(*redirection)->in_file);
	ft_expand_outfile(&(*redirection)->out_file);
}

t_tnode	*ft_new_tnode(int n_type, t_token *tokens)
{
	t_tnode *new;
	

	printf("===== parse command ====new node\n");
	var_dump_token(tokens);
	printf("===== parse command ====new node\n");
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
		new->redirection = NULL;
		if (is_parenthise_redirection(tokens))
		{	
			while (tokens && tokens->typeId != 5)
				tokens = tokens->next;
			 ftGetRedirection(&new->redirection ,tokens->next, 1 , 1);
		}
	}
	else
	{
		new->cmd = ftGenCmd(tokens);
		new->redirection = NULL;
		ftGetRedirection(&new->redirection, tokens,1, 1);
		if ((!new->redirection->in_file || !new->redirection->out_file))
		{
			if (is_parenthise_redirection(tokens))
			{	
				printf(":( parenthise)========\n");
				while (tokens && tokens->typeId != 5)
					tokens = tokens->next;
				// printf("(%d)==(%s)\n", tokens->typeId, tokens->next->value);
				if (!new->redirection->in_file)
					ftGetRedirection( &new->redirection, tokens->next, 1 ,0);
				if (!new->redirection->out_file)
				ftGetRedirection(&new->redirection, tokens->next, 0 , 1);
			}
		}
	}
	return (new);
}

void ft_parse_parenthise(t_tnode **root, t_token **tokens)
{
    t_token *tmp;
    t_tnode *new;
    t_token *tParenthise;
    t_token *tmp1;

    tmp = *tokens;
    while (tmp && tmp->value && !is_l_parenthise(tmp->value[0]))
        tmp = tmp->next;
	free(tmp->value);
	free(tmp->next->next->value);
	tmp->value = NULL;
	tmp->next->next->value = NULL;
	tParenthise = ft_tokinizer(tmp->next->value);
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
	while (tmp)
	{
		if (tmp->value && is_pipe(tmp->value[0]) && !ft_check_pipe(tmp->next))
			break;
		tmp = tmp->next;
	}
	new = ft_new_tnode(1, *tokens);
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
	ft_parse_ast(&new, tokens);
	ft_parse_ast(&new, &tmp->next);	
}

void	ft_parse_or_operator(t_tnode **root, t_token **tokens)
{
	t_token	*tmp;
	t_tnode	*new;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->value && is_pipe(tmp->value[0]) && is_pipe(tmp->value[1]) && isLastOperator(tmp->next))
			break;
		tmp = tmp->next;
	}
	new = ft_new_tnode(2, *tokens);
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
	ft_parse_ast(&new, tokens);
	ft_parse_ast(&new, &(tmp->next));
}


void	ft_parse_and_operator(t_tnode **root, t_token **tokens)
{
	t_token *tmp;
	t_tnode	*new;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->value && is_operator(tmp->value[0]) && isLastOperator(tmp->next))
			break;
		tmp = tmp->next;
	}
	new = ft_new_tnode(3, *tokens);
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
	ft_parse_ast(&new, tokens);
	ft_parse_ast(&new, &tmp->next);
}

void	ft_parse_cmd(t_tnode **root, t_token **tokens)
{
	t_tnode *new;
	

	new = ft_new_tnode(0, *tokens);
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
}

void ft_parse_ast(t_tnode **root, t_token **tokens)
{
	t_token *tmp;
	int i;

	i = 0;
	tmp = *tokens;
	while (tmp && tmp->value)
	{
		if (ft_check_and_operator(tmp))
			return (ft_parse_and_operator(root, tokens));
		if (ft_check_or_operator(tmp))
			return (ft_parse_or_operator(root, tokens));
		if (ft_check_pipe(tmp))
			return (ft_parse_pipe(root, tokens));
		if (ft_check_parenthises(tmp))
			return (ft_parse_parenthise(root, tokens));
		return ft_parse_cmd(root, tokens);
		tmp = tmp->next;
	}
} 