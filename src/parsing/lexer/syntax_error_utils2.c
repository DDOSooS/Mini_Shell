# include "../../../includes/mshell.h"

int ft_check_empty_parenthise(char *cmd_line)
{
    int i;
    int len;

    i = 0;
    len = ft_strlen(cmd_line);
    while (i < len)
    {
        if (is_l_parenthise(cmd_line[i]))
        {
            i++;
            while (cmd_line[i] && is_whites_space(cmd_line[i]))
                i++;
            if (is_r_parenthise(cmd_line[i]) || is_logical_operator(&cmd_line[i]) || is_pipe(cmd_line[i]))
			    return (ft_printf("syntax error near unexpected token `()'\n"), 0);
        }
        i++;
    }
    return (1);
}
