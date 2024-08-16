# include "../../../includes/mshell.h"


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