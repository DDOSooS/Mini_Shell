/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 20:05:21 by aghergho          #+#    #+#             */
/*   Updated: 2024/06/10 23:21:12 by aghergho         ###   ########.fr       */
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
	new->arg = ft_strdup(str);
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
	if (!*root)
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
	// ft_printf("------------------------------------------cmd(%s)---------------------------------------\n\n", tokens->value);
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


int ftAddRedirection(t_redirection **root, t_token *token)
{
	if (token->typeId == 7 || token->typeId == 8)
		ftAddInFile(&(*root)->in_file, token);
	if (token->typeId ==6 || token->typeId == 9)
		ftAddOutFile(&(*root)->out_file, token);
	return (1);
}

t_redirection *ftGetRedirection(t_token *token)
{
    int             flag;
    t_redirection   *redirection;

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


// =========================================================count the expanded args's lesn ==================================================================

int ft_check_env_var(char *str)
{
	t_env *tmp;

	tmp = g_mshell.env;
	ft_printf("=== expanded token (%s)====wydad===\n", str);
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, str))
		{
			printf("expande tokennn jjj:(%s)len (%d)<<<<<<\n", tmp->value, ft_strlen(tmp->value));
			return (ft_strlen(tmp->value));
		}
		tmp = tmp->next;
	}
	return (0);
}

int ft_count_number_len(pid_t number)
{
	int counter;

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
	// ft_printf("+++++++++++++++++++++++++++++++ search Counter (%d) +++++++++++++++++++++++++++++=\n", *counter);
	while (token[++i] && !is_quote(token[i + 1]) && !is_whites_space(token[i + 1]) && !ft_check_quote(token, i + 1));
	str = ft_substr(token, 1, i);
	start = ft_check_env_var(str);
	(*counter) = (*counter) + start;
	// ft_printf("-->>>>>>>>>>>>>>>>---Expanded search Token --------(%d)-(%d)||--(%c)------;)))))))---\n\n", i,*counter, token[i]);
	free(str);
	return (i);
}
// laskjfdas3419318HOMEadfskldsaf
int ft_get_unexpanded_token(char *token, int *counter)
{
	int i;
	
	i = 0 ;

	// ft_printf("--------------------unexpanded TOOOOOOOOOOOOOOOOOOOOOKEN--------------------\n\n");
	// ft_printf("-------------------------------(%s)------------------------------\n", token);
	while (token[++i] && ft_check_quote(token, i + 1))
		(*counter)++;
	// ft_printf("--------------------unexpanded --------(%d)-(%d)--(%c)---------\n\n", i, *counter, token[i]);
	// ft_printf("--------------------unexpanded TOOOOOOOOOOOOOOOOOOOOOKEN--;((((((((((((------(%d)-(%d)--(%c)------;)))))))---\n\n", i, *counter, token[i]);
	return (i)	;
}


int ft_get_expanded_quoted_token(char *token, int *counter)
{
	int start;
	int i;

	i = 0;
	// ft_printf("================================(expanded Quoted ToKen)===(%c)=========COUNTER(%d)=========================\n", token[i], *counter);
	while (token[++i] && ft_check_quote(token, i + 1))
	{
		// ft_printf("=========TOKEN[%c]----------------------------\n\n", token[i]);
		if (!is_dollar_sign(token[i]))
		{
			(*counter)++;
			continue;
		}
		if (is_dollar_sign(token[i]) && token[i + 1] && is_dollar_sign(token[i + 1]))
		{
			(*counter) += ft_count_number_len(g_mshell.pid);
			i++;
		}
		else if (is_dollar_sign(token[i]) && token[i + 1])
		{
			i += ft_search_expanded_token(&token[i], counter);
		}
	}
	ft_printf("\n============================= Current Counter (%d)[%c]=========================================\n\n", *counter, token[i]);
	return (i);
}

// laskjfdas"'$HOME'"'adfskl''dsaf'

int ft_get_expanded_unquoted_token(char *token, int *counter)
{
	int		i;
	char	*str;
	
	i = 0;
	if (token[i + 1] && is_dollar_sign(token[i + 1]))
	{
		(*counter) += ft_count_number_len(g_mshell.pid);
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
	ft_printf(">>>>>>> IS $$            (%s)   <<<<<<<<<<(%d)<<<<<<<\n",str, *counter );
	free(str);
	return (i);
}


int ft_count_expanded_token(char *token, int *counter)
{
	int i;

	i = -1;
	while (token[++i])
	{
		// ft_printf("-----token >>> (%s) <<<<  [%c][%d]Counter(%d)------\n\n",token, token[i], i, *counter);
		if (is_single_quote(token[i]))
		{
			// ft_printf("here<<CASE1<<<<<<(%d)(%d)<<<<<<COUNTER>>>(%c)<<<<\n",*counter, i, token[i]);
			i += ft_get_unexpanded_token(&token[i ], counter);
			return (i);
			// ft_printf("\n\n UNexpanded  token (CASE 1)(%d)(%c)  \n\n", i, token[i]);
			// if (!token[i + 1])
				// ft_printf("yes w're at the last charactere+++++++++++++++++++++++++++++++++++++++++++==\n");			
		}
		else if (is_quote(token[i]))
		{
			// ft_printf("-----token[%c][%d]Counter(%d)----CASE2222--\n\n", token[i], i, *counter);
			i += ft_get_expanded_quoted_token(&token[i], counter) ;
			return (i);
			// ft_printf("\n\n expanded  token Whith Quotes $$ DOLLAR (CASE 2)(%c)<<<<<<<<<<<\n\n", token[i]);
		}
		else if (is_dollar_sign(token[i]))
		{
			// ft_printf("-----token[%c][%d]Counter(%d)----CASE33333--\n\n", token[i], i, *counter);
			// ft_printf("\n\n+++++++++++++++++ expanded  token without Quotes $$ DOLLAR (CASE 3)(%d)(%c)~~~~(%s)++++++++++++++++++++++\n\n",i , token[i], &token[i]);
			i += ft_get_expanded_unquoted_token(&token[i], counter);
			return (i);
			// ft_printf("-----token after expand[%c][%d]Counter(%d)------\n\n", token[i], i, *counter);
			
		}
		// else
		// 	(*counter) += 1;
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
		{
			i += ft_count_expanded_token(&token[i], &counter);		
			// ft_printf("*********---------------**(%c)*(%d)********-------------**********\n", token[i], i);	
		}
		else
		{
			// ft_printf("********************(%c)*(%d)************************************\n", token[i], i);
			counter++;
		}
	}
	return (counter);
}



// =========================================================generate the expanded args ==================================================================

int ft_gen_pid_token(char **str, pid_t pid)
{
	char *tmp;

	tmp = ft_itoa((int)pid);
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
	// ft_printf("=== expanded token (%s)====wydad===\n", str);
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, str))
		{
			// printf("expande tokennn jjj:(%s)len (%d)<<<<<<\n", tmp->value, ft_strlen(tmp->value));
			return (tmp->value);
		}
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
	if (token[i + 1] && is_dollar_sign(token[i + 1]))
	{
		ft_gen_pid_token(s1, g_mshell.pid);
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
	// ft_printf("+++++++++++++++++++++++++++++++ search Counter (%d) +++++++++++++++++++++++++++++=\n", *counter);
	while (token[++i] && !is_quote(token[i + 1]) && !is_whites_space(token[i + 1]) && !ft_check_quote(token, i + 1));
	str = ft_substr(token, 1, i);
	env_len = ft_check_env_var(str);
	if (env_len)
	{
		tmp = ft_get_env_var(str);
		*s1 = ft_strcat(*s1, tmp);
	}
	// ft_printf("-->>>>>>>>>>>>>>>>---Expanded search Token --------(%d)-(%d)||--(%c)------;)))))))---\n\n", i,*counter, token[i]);
	free(str);
	return (i);
}


int ft_gen_expanded_quoted_token(char **str, char *token)
{
	int i;

	i = 0;
	// ft_printf("================================(expanded Quoted ToKen)===(%c)=========COUNTER(%d)=========================\n", token[i], *counter);
	while (token[++i] && ft_check_quote(token, i + 1))
	{
		// ft_printf("=========TOKEN[%c]----------------------------\n\n", token[i]);
		if (!is_dollar_sign(token[i]))
		{
			*str = ft_strcat_char(*str, token[i]);
			continue;
		}
		if (is_dollar_sign(token[i]) && token[i + 1] && is_dollar_sign(token[i + 1]))
		{
			ft_gen_pid_token(str, g_mshell.pid);
			i++;
		}
		else if (is_dollar_sign(token[i]) && token[i + 1])
		{
			i += ft_gen_search_expanded_token(str,&token[i]);
		}
	}
	// ft_printf("\n============================= Current Counter (%d)[%c]=========================================\n\n", *counter, token[i]);
	return (i);
}

int ft_gen_unexpanded_token(char **str, char *token)
{
	int i;
	
	i = 0 ;

	// ft_printf("--------------------unexpanded TOOOOOOOOOOOOOOOOOOOOOKEN--------------------\n\n");
	// ft_printf("-------------------------------(%s)------------------------------\n", token);
	while (token[++i] && ft_check_quote(token, i + 1))
		*str = ft_strcat_char(*str, token[i]);
	// ft_printf("--------------------unexpanded --------(%d)-(%d)--(%c)---------\n\n", i, *counter, token[i]);
	// ft_printf("--------------------unexpanded TOOOOOOOOOOOOOOOOOOOOOKEN--;((((((((((((------(%d)-(%d)--(%c)------;)))))))---\n\n", i, *counter, token[i]);
	return (i)	;
}



int ft_gen_expanded_token(char **str,char *token)
{
	int i;

	i = -1;
	while (token[++i])
	{
		// ft_printf("-----token[%c][%d]Counter(%d)------\n\n", token[i], i, *counter);
		if (is_single_quote(token[i]))
		{
			// ft_printf("here<<CASE1<<<<<<(%d)(%d)<<<<<<COUNTER>>>(%c)<<<<\n",*counter, i, token[i]);
			i += ft_gen_unexpanded_token(str, &token[i]);
			return (i);
			// ft_printf("\n\n UNexpanded  token (CASE 1)(%d)(%c)  \n\n", i, token[i]);
			// if (!token[i + 1])
			// 	ft_printf("yes w're at the last charactere+++++++++++++++++++++++++++++++++++++++++++==\n");			
		}
		else if (is_quote(token[i]))
		{
			// ft_printf("-----token[%c][%d]Counter(%d)----CASE2222--\n\n", token[i], i, *counter);
			i += ft_gen_expanded_quoted_token(str, &token[i]) ;
			return (i);
			// ft_printf("\n\n expanded  token Whith Quotes $$ DOLLAR (CASE 2)(%c)<<<<<<<<<<<\n\n", token[i]);
		}
		else if (is_dollar_sign(token[i]))
		{
			// ft_printf("-----token[%c][%d]Counter(%d)----CASE33333--\n\n", token[i], i, *counter);
			// ft_printf("\n\n+++++++++++++++++ expanded  token without Quotes $$ DOLLAR (CASE 3)(%d)(%c)~~~~(%s)++++++++++++++++++++++\n\n",i , token[i], &token[i]);
			i += ft_gen_expanded_unquoted_token(str, &token[i]);
			return (i);
			// ft_printf("-----token after expand[%c][%d]Counter(%d)------\n\n", token[i], i, *counter);
			
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
		{
			i += ft_gen_expanded_token(str, &token[i]);		
			// ft_printf("*********---------------**(%c)*(%d)********-------------**********\n", token[i], i);	
		}
		else
		{
			// ft_printf("********************(%c)*(%d)************************************\n", token[i], i);
			counter++;
			*str = ft_strcat_char(*str , token[i]);
		}
	}
	return (counter);
}


void ft_expand_arg(char **arg)
{
	int len;
	char *tmp;
	char *new;
	// ft_printf("cmd (%s)==========\n", *arg);
	tmp = *arg;
	len = ft_expanded_token(tmp);
	// ft_printf("lennnnnnnnnnnnnnnnnnnnnnnnn (%d)==========\n", len);
	new = malloc(sizeof(char) * (len +  1));
	new[0] = '\0';
	ft_gen_expanded_arg(&new, tmp);
	free(*arg);
	*arg = new;
}


void ft_expand_cmd(t_cmd **root)
{
	t_cmd	*tmp;

	tmp = *root;
	while (tmp)
	{
		if (ft_check_expand(tmp->arg))
		{
			// ft_printf("yes we gonna expand it --(%s)-----------------------\n", tmp->arg);
			ft_expand_arg(&tmp->arg);
		}
		tmp = tmp->next;
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++expand of out files++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


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

	// ft_printf("--------------------unexpanded TOOOOOOOOOOOOOOOOOOOOOKEN--------------------\n\n");
	// ft_printf("-------------------------------(%s)------------------------------\n", token);
	while (token[++i] && ft_check_quote(token, i + 1))
	{
		(*counter)++;
	}
	// ft_printf("--------------------unexpanded --------(%d)-(%d)--(%c)---------\n\n", i, *counter, token[i]);
	// ft_printf("--------------------unexpanded TOOOOOOOOOOOOOOOOOOOOOKEN--;((((((((((((------(%d)-(%d)--(%c)------;)))))))---\n\n", i, *counter, token[i]);
	return (i)	;
}

int ft_count_expanded_deliter(char *token, int *counter)
{
	int i;

	i = -1;
	while (token[++i])
	{
		// ft_printf("-----token >>> (%s) <<<<  [%c][%d]Counter(%d)------\n\n",token, token[i], i, *counter);
		if (is_single_quote(token[i]))
		{
			// ft_printf("here<<CASE1<<<<<<(%d)(%d)<<<<<<COUNTER>>>(%c)<<<<\n",*counter, i, token[i]);
			i += ft_get_unexpanded_delimiter(&token[i ], counter);
			return (i);
			// ft_printf("\n\n UNexpanded  token (CASE 1)(%d)(%c)  \n\n", i, token[i]);
			// if (!token[i + 1])
				// ft_printf("yes w're at the last charactere+++++++++++++++++++++++++++++++++++++++++++==\n");			
		}
		else if (is_quote(token[i]))
		{
			// ft_printf("-----token[%c][%d]Counter(%d)----CASE2222--\n\n", token[i], i, *counter);
			i += ft_get_expanded_quoted_delimiter(&token[i], counter) ;
			return (i);
			// ft_printf("\n\n expanded  token Whith Quotes $$ DOLLAR (CASE 2)(%c)<<<<<<<<<<<\n\n", token[i]);
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
		{
			i += ft_count_expanded_deliter(&token[i], &counter);		
			// ft_printf("*********---------------**(%c)*(%d)********-------------**********\n", token[i], i);	
		}
		else
		{
			// ft_printf("********************(%c)*(%d)************************************\n", token[i], i);
			counter++;
		}
	}
	return (counter);
}


//____________________________________________________________________________________Generating the delimiter__________________________________________________

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
	// ft_printf("--------------------unexpanded TOOOOOOOOOOOOOOOOOOOOOKEN--------------------\n\n");
	// ft_printf("-------------------------------(%s)------------------------------\n", token);
	while (token[++i] && ft_check_quote(token, i + 1))
		*str = ft_strcat_char(*str, token[i]);
	// ft_printf("--------------------unexpanded --------(%d)-(%d)--(%c)---------\n\n", i, *counter, token[i]);
	// ft_printf("--------------------unexpanded TOOOOOOOOOOOOOOOOOOOOOKEN--;((((((((((((------(%d)-(%d)--(%c)------;)))))))---\n\n", i, *counter, token[i]);
	return (i)	;
}

int ft_gen_expanded_deliter(char **str, char *token)
{
	int i;

	i = -1;
	while (token[++i])
	{
		// ft_printf("-----token >>> (%s) <<<<  [%c][%d]Counter(%d)------\n\n",token, token[i], i, *counter);
		if (is_single_quote(token[i]))
		{
			// ft_printf("here<<CASE1<<<<<<(%d)(%d)<<<<<<COUNTER>>>(%c)<<<<\n",*counter, i, token[i]);
			i += ft_gen_unexpanded_delimiter(str, &token[i ]);
			return (i);
			// ft_printf("\n\n UNexpanded  token (CASE 1)(%d)(%c)  \n\n", i, token[i]);
			// if (!token[i + 1])
				// ft_printf("yes w're at the last charactere+++++++++++++++++++++++++++++++++++++++++++==\n");			
		}
		else if (is_quote(token[i]))
		{
			// ft_printf("-----token[%c][%d]Counter(%d)----CASE2222--\n\n", token[i], i, *counter);
			i += ft_gen_expanded_quoted_delimiter(str, &token[i]) ;
			return (i);
			// ft_printf("\n\n expanded  token Whith Quotes $$ DOLLAR (CASE 2)(%c)<<<<<<<<<<<\n\n", token[i]);
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
		{
			i += ft_gen_expanded_deliter(str ,&token[i]);		
			// ft_printf("*********---------------**(%c)*(%d)********-------------**********\n", token[i], i);	
		}
		else
		{
			// ft_printf("********************(%c)*(%d)************************************\n", token[i], i);
			*str = ft_strcat_char(*str, token[i]);
		}
	}
	return (1);
}



void ft_expand_delimiter(char **arg)
{
	int len;
	char *tmp;
	char *new;
	// ft_printf("Delmiter (%s)==========\n", *arg);
	tmp = *arg;
	len = ft_count_delimter_len(tmp);
	// ft_printf("lennnnnnnnnnnnnnnnnnnnnnnnn (%d)==========\n", len);
	new = malloc(sizeof(char) * (len +  1));
	new[0] = '\0';
	ft_gen_delimter(&new, tmp);
	free (tmp);
	*arg = new;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void ft_expand_infile(t_infile **root)
{
	t_infile *tmp;

	tmp = *root;
	while (tmp)
	{
		if (tmp->is_herdoc)
			ft_expand_delimiter(&tmp->filename);
		else
			ft_expand_arg(&tmp->filename);
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
			while (tokens && tokens->typeId != 5)
				tokens = tokens->next;
			new->redirection = ftGetRedirection(tokens->next);
		}
		else
			new->redirection = NULL;
	}
	else
	{
		new->cmd = ftGenCmd(tokens);
		ft_expand_cmd(&(new->cmd));
		new->redirection = ftGetRedirection(tokens);
		ft_expand_redirection(&(new->redirection));
	}
	return (new);
}
		// new->cmd = ft_gen_cmd(tokens);
 
	// ft_printf("yes>>>>>>>>>>>>>>>>>>>>>>there is<<<<<<<\n");
	// ft_printf("===============>>>>>>>>>ADDDDDD TOOOOKEN<<<<<<<<<=========================\n");
	// var_dump_token(tokens);
	// ft_printf("<><><><><<><><><><><><><><><><><><><><><><>><><><><>><><><><>\n\n\n");

		// ft_printf("------done redirections -----\n");
		// varDumpOutFile(new->redirection->out_file);
		// varDumpInFile(new->redirection->in_file);

		// new->redirection = NULL;
		// if (!new->redirection)
		// 	return(free(new->cmd), free(new), NULL);
		// new->cmd = ft_expand_cmd(&new->cmd);
		// ft_printf("===============>>>>>>>>>ADDDDDD TOOOOKEN<<<<<<<<<=========================\n");
		// var_dump_token(tokens);
		// ft_printf("<><><><><<><><><><><><><><><><><><><><><><>><><><><>><><><><>\n\n\n");
		// if (! new->cmd)
		// 	return (free(new), NULL);
		// ft_printf("------done command -----\n");

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