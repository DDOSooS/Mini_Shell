# include "../../../includes/mshell.h"



int out_redirection(char c)
{
    if (c == '>')
        return (1);
    return (0);
}

int is_pipe(char c)
{
    if (c == '|')
        return (1);
    return (0);
}

int is_operator(char c)
{
    if (c == '&')
        return (1);
    return (0);
}

int is_r_parenthise(char c)
{
    if (c == ')')
        return (1);
    return (0);
}

int is_l_parenthise(char c)
{
    if (c == '(')
        return (1);
    return (0);
}