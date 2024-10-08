
# include "../../../includes/mshell.h"

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


int ft_search_expanded_token(char *token, int *counter)
{
	int i;
	int start;
	char *str;
	
	i = 0;
	start = 1;
	while (token[++i] && !is_quote(token[i + 1]) && !is_whites_space(token[i + 1]) && !ft_check_quote(token, i + 1));
	str = ft_substr(token, 1, i);
	start = ft_check_env_var(str);
	(*counter) = (*counter) + start;
	free(str);
	return (i);
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


int ft_get_expanded_quoted_token(char *token, int *counter)
{
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
			(*counter)++;
	}
	return (i);
}


