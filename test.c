# include <stdio.h>
#include <string.h>
int ft_check_quote_error(char *cmd_line)
{
	int d_quote;
	int s_quote;
	int i;
	int len;
	
	i = 0;
	len = strlen(cmd_line);
	d_quote = 0;
	s_quote = 0;
	while (i < len)
	{

		if (cmd_line[i] == 34 && !s_quote && d_quote)
			d_quote = 1;
        else if (cmd_line[i] == 34 && d_quote)
            d_quote = 0;
		if(cmd_line[i] == 39 && !d_quote && ! s_quote)
			s_quote = 1;
        else if (cmd_line[i] == 39 && s_quote)
            s_quote = 0;
		i++;
	}
    printf("===> s_quote(%d)|d_quote(%d)\n", s_quote, d_quote);
	if (d_quote || s_quote)
		return (1);
	return (0);
}

int main(int ac , char **av)
{
    printf ("syntax error:? %d", ft_check_quote_error(av[1]));
    return (0);
}
