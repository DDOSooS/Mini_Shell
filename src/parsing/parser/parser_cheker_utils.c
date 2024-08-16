# include "../../../includes/mshell.h"

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
