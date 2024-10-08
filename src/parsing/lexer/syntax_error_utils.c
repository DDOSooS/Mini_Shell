# include "../../../includes/mshell.h"



int ft_check_operation_error(char *cmd)
{
	int i;
	int	len;

	i = 0;
	len = ft_strlen(cmd);
	while ( i < len && (cmd[i] == ' ' || cmd[i] == '\t'))
			i++;
    if (cmd[i] && (cmd[i] == '|' || cmd[len-1] == '|'))
        return (ft_putstr_fd("parse error near `|'\n", 2), 1);
    if (! ft_pipe_sequence_error(cmd))
        return (ft_putstr_fd("parse error near `|'\n", 2), 1);
    if (cmd[i] && (cmd[i] == '&' || cmd[len-1] == '&'))
        return (ft_putstr_fd("parse error near `&'\n", 2), 1);
    if (! ft_operatore_sequence_error(cmd))
            return (ft_putstr_fd("parse error near `&'\n", 2), 1);
	return (0);
}
 
int ft_check_quote_error(char *cmd_line)
{
	int d_quote;
	int s_quote;
	int i;
	int len;
	
	i = -1;
	len = ft_strlen(cmd_line);
	d_quote = 0;
	s_quote = 0;
	while (++i < len)
	{
		if (cmd_line[i] == 34 && !s_quote && !d_quote)
			d_quote = 1;
        else if (cmd_line[i] == 34 && d_quote)
            d_quote = 0;
		if(cmd_line[i] == 39 && !d_quote && ! s_quote)
			s_quote = 1;
        else if (cmd_line[i] == 39 && s_quote)
            s_quote = 0;
	}
	if (d_quote || s_quote)
		return (ft_printf(";( Syntax error please verify your quotes\n"),1);
	return (0);
}

int ft_check_redirection_sequence(char *cmd_line)
{
    int i;
    int len;

    i = -1;
    len = ft_strlen(cmd_line);
    while (++i < len)
    {
        if (cmd_line[i] == '>' && cmd_line[i + 1] && cmd_line[i + 1] == '>'
            && cmd_line[i + 2] && cmd_line[i + 2] == '>' &&  !ft_check_quote(cmd_line, i))
        {
            perror("parse error near `>'\n");
            return (1);
        }
        if (cmd_line[i] == '<' && cmd_line[i + 1] && cmd_line[i + 1] == '<'
            && cmd_line[i + 2] && cmd_line[i + 2] == '<' &&  !ft_check_quote(cmd_line, i))
        {
            perror("parse error near `<'\n");
            return (1);
        }
    }
    return (0);
}

int ft_check_file_name(char *cmd_line)
{
    int i;
    
    i = -1;
    while (cmd_line[++i] && is_whites_space(cmd_line[i]));
    if (!cmd_line[i])
        return (ft_printf("parse error near `\\n'\n"),0);
    if (is_operator(cmd_line[i]) && cmd_line[i + 1] && is_operator(cmd_line[i + 1]))
        return (ft_printf("parse error near `&&'\n"), 0);
    if (is_operator(cmd_line[i]))
        return (ft_printf("parse error near `&'\n"), 0);
    else if (is_pipe(cmd_line[i]) && cmd_line[i + 1] && is_pipe(cmd_line[i + 1]))
        return (ft_printf("parse error near `||'\n"), 0);
    else if (is_pipe(cmd_line[i]) )
        return (ft_printf("parse error near `|'\n"), 0);
    else if (in_redirection(cmd_line[i]))
        return (ft_printf("parse error near `<'\n"), 0);
    else if (out_redirection(cmd_line[i]))
        return (ft_printf("parse error near `>'\n"), 0);
    else if (is_r_parenthise(cmd_line[i]) || is_r_parenthise(cmd_line[i]))
        return (ft_printf("parse error near `)'\n"), 0);
    return (1);
}

int ft_check_in_redirection(char *cmd_line , int len)
{
    int i;
    
    i = -1;
    while (++i < len)
    {
        if (in_redirection(cmd_line[i]) && ! ft_check_quote(cmd_line, i))
        {
            if (cmd_line[i + 1] && in_redirection(cmd_line[i + 1]) && !ft_check_quote(cmd_line, i))
            {
                if (!ft_check_file_name(&cmd_line[i + 2]))
                   return (0);
            }
            else
            {
                if (!ft_check_file_name(&cmd_line[i + 1]))
                    return (0);
            }
        }
    }
    return (1);
}
