# include "../../../includes/mshell.h"

int check_dollar_sign_quoted(char *token, int index)
{
    return (is_dollar_sign(token[index])  && is_quote(token[index + 1])
        && !is_dollar_sign(token[index - 1]) && !ft_check_quote(token ,index + 1));
}

int check_closed_quote_sequence(char *token, int index)
{
    return (is_quote(token[index]) && token[index + 1] &&
        token[index] == token[index + 1] && !ft_check_quote(token, index + 2));
}

int ft_check_dollar(char *token)
{
    int i;

    i = -1;
    while (token[++i])
    {
        if (token[i] == '$' && token[i +1])
            return (1);      
    }
    return (0);
}


int ft_check_expand(char *tmp)
{
    if (is_exist_quote(tmp) || ft_check_dollar(tmp))
        return (1);
    return (0);
}

int ft_check_expand_delimiter(char *delimiter)
{
    if (delimiter && delimiter[0] == '$')
        return (1);
    return (0);
}