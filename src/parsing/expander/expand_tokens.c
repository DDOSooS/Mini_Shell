# include "../../../includes/mshell.h"

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

int	ft_expanded_token_len(char *token)
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

int ft_expand_arg(char **arg)
{
	int len;
	char *tmp;
	char *new;

	tmp = *arg;
	len = ft_expanded_token_len(tmp);
	new = malloc(sizeof(char) * (len +  1));
	if (!new)
		return (0);
	new[0] = '\0';
	ft_gen_expanded_arg(&new, tmp);
	free(*arg);
	*arg = new;
	return (1);
}
