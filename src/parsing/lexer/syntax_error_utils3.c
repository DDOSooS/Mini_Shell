/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:00:48 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/26 16:33:41 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_check_out_redirection(char *cmd_line, int len)
{
	int	i;

	i = -1;
	while (++i < len)
	{
		if (out_redirection(cmd_line[i]) && !ft_check_quote(cmd_line, i))
		{
			if (cmd_line[i + 1] && out_redirection(cmd_line[i + 1])
				&& !ft_check_quote(cmd_line, i))
			{
				if (!ft_check_file_name(&cmd_line[i + 2]))
					return (0);
			}
			else if (!ft_check_file_name(&cmd_line[i + 1]))
				return (0);
		}
	}
	return (1);
}

int	ft_check_redirection_error(char *cmd_line)
{
	int	len;

	len = ft_strlen(cmd_line);
	if (ft_check_redirection_sequence(cmd_line))
		return (0);
	if (!ft_check_in_redirection(cmd_line, len))
		return (0);
	if (!ft_check_out_redirection(cmd_line, len))
		return (0);
	return (1);
}

int	ft_check_parenthise(char *cmd_line)
{
	int	i;

	i = 0;
	while (cmd_line[i] && is_whites_space(cmd_line[i]))
		i++;
	if (cmd_line[i] && is_r_parenthise(cmd_line[i]))
		return (ft_putstr_fd("syntax error near unexpected token `)'\n"
				, 2), 0);
	if (cmd_line[i] && (!is_operator(cmd_line[i]) || !is_pipe(cmd_line[i])
			|| !is_r_parenthise(cmd_line[i]) || !is_l_parenthise(cmd_line[i])))
	{
		if (is_l_parenthise(cmd_line[i]))
			return (ft_putstr_fd("syntax error near unexpected token `('\n"
					, 2), 0);
		else
			return (ft_putstr_fd("syntax error near unexpected token `)'\n"
					, 2), 0);
	}
	return (1);
}

int	ft_check_closed_parenthise(char *cmd_line)
{
	int	i;
	int	len;
	int	closed_parenth;

	len = ft_strlen(cmd_line);
	closed_parenth = 0;
	i = -1;
	while (++i < len)
	{
		if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i)
			&& !closed_parenth)
			return (ft_putstr_fd("parse error near `)'\n", 2), 0);
		else if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
			closed_parenth--;
		if (is_l_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
			closed_parenth++;
	}
	if (closed_parenth)
	{
		ft_putstr_fd("parenthisec are not closed correctly\n", 2);
		return (0);
	}
	return (1);
}

int	is_closed_parenthise(char *cmd_line, int len)
{
	int	i;
	int	closed_parenth;

	closed_parenth = 0;
	i = -1;
	while (++i < len)
	{
		if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i)
			&& !closed_parenth)
			return (0);
		if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
			closed_parenth--;
		if (is_l_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
			closed_parenth++;
	}
	if (closed_parenth)
		return (0);
	return (1);
}
