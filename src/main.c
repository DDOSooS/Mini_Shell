/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:02:56 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/11 08:46:33 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/mshell.h"



/*
	@var_dump function is a function that debug double 2d char variable
*/
void	var_dump(char **str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		ft_printf("==>str[%d]:[%s]\n", i, str[i]);
		i++;
	}
}
int main()
{
	char	*cmd_line;
	
	while (1)
	{
			cmd_line = readline("minishell ;)>  ");
			// ft_printf("%s\n", cmd_line);
			// char **args = ft_split(cmd_line, '|');
			// var_dump(args);
			// ft_parse_input(cmd_line);
			ft_check_syntax(cmd_line);
			if (ft_strcmp(cmd_line, "exit") == 0)
			{
				free(cmd_line);
				return (0);
			}
			free(cmd_line);
	}
    return (0);
}
