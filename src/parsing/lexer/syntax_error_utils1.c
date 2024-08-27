/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:31:26 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 02:43:41 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	is_word_character(char c)
{
	if (is_pipe(c) || is_operator(c) || in_redirection(c) || out_redirection(c))
		return (0);
	return (1);
}

int	ft_check_left_parenthise(char *cmd_line, int i)
{
	while (--i >= 0 && cmd_line[i] && is_l_parenthise(cmd_line[i]))
		;
	while (--i >= 0 && cmd_line[i] && !is_pipe(cmd_line[i])
		&& !is_operator(cmd_line[i]))
	{
		if (is_l_parenthise(cmd_line[i]))
		{
			i--;
			continue ;
		}
		if (!is_whites_space(cmd_line[i]) && cmd_line[i])
			return (0);
		i--;
	}
	return (1);
}

int	is_redirection(char c)
{
	if (in_redirection(c) || out_redirection(c))
		return (1);
	return (0);
}

void	ft_check_redirection(char c, int *redirection_found)
{
	if (is_redirection(c))
		*redirection_found = 1;
}

int	ft_count_file_name(char *cmd, int index)
{
	int	counter;

	counter = 0;
	while (cmd[index] && !is_whites_space(cmd[index]) && !is_pipe(cmd[index])
		&& !is_operator(cmd[index]))
	{
		counter++;
		index++;
	}
	return (counter);
}
