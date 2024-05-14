/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:02:56 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/14 14:24:40 by aghergho         ###   ########.fr       */
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

void	var_dump_token(t_token *tokens)
{
	while (tokens)
	{
		ft_printf("==type===%s-----\n",tokens->type);
		ft_printf("===value==%s\n================\n",tokens->value);
		tokens = tokens->next;
	}
	
}
int main()
{
	char	*cmd_line;
	t_token *tokens;
	while (1)
	{
			cmd_line = readline("minishell ;)>  ");
			ft_check_syntax(cmd_line);
			tokens = ft_tokinizer(cmd_line);
			var_dump_token(tokens);
			if (ft_strcmp(cmd_line, "exit") == 0)
				return (free(cmd_line),0);
			free(cmd_line);
	}
    return (0);
}
