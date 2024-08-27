/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:51:21 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 02:43:45 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	is_white_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

int	check_white_spaces(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (is_white_space(cmd[i]) != 1)
			return (1);
		i++;
	}
	return (0);
}

int	check_is_last_parenthis(char *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return (0);
	while (cmd[i])
	{
		while (is_whites_space(cmd[i]))
			i++;
		if (is_l_parenthise(cmd[i]))
			return (0);
		else
			return (1);
		i++;
	}
	return (1);
}

int	ft_check_empty_parenthise(char *cmd_line)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(cmd_line);
	while (i < len)
	{
		if (is_l_parenthise(cmd_line[i])
			&& check_is_last_parenthis(&cmd_line[i + 1]))
		{
			i++;
			while (cmd_line[i] && is_whites_space(cmd_line[i]))
				i++;
			if (is_r_parenthise(cmd_line[i])
				|| is_logical_operator(&cmd_line[i]) || is_pipe(cmd_line[i]))
				return (ft_putstr_fd("syntax error near unexpected token `)'\n"
						, 2), 0);
		}
		i++;
	}
	return (1);
}
