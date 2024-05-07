/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:02:56 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/07 14:34:03 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/mshell.h"

int main(int ac, char **av)
{
	char	*cmd_line;
	
	while (1)
	{
			cmd_line = readline("minishell ;)>  ");
			ft_printf("%s\n", cmd_line);
			if (ft_strcmp(cmd_line, "exit") == 0)
			{
				free(cmd_line);
				return (0);
			}
			free(cmd_line);
	}
	if (ac == 2) 
		ft_printf("%s", av[1]);
    return (0);
}
