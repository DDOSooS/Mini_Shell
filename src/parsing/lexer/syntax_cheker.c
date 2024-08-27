/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_cheker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:50:48 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 19:00:46 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_check_quote(char *cmd_line, int len)
{
	int	d_quote;
	int	s_quote;
	int	i;

	i = -1;
	d_quote = 0;
	s_quote = 0;
	while (++i < len && cmd_line[i])
	{
		if (cmd_line[i] == 34 && !s_quote && !d_quote)
			d_quote = 1;
		else if (cmd_line[i] == 34 && d_quote)
			d_quote = 0;
		if (cmd_line[i] == 39 && !d_quote && !s_quote)
			s_quote = 1;
		else if (cmd_line[i] == 39 && s_quote)
			s_quote = 0;
	}
	if (d_quote)
		return (1);
	if (s_quote)
		return (2);
	return (0);
}

int	ft_check_pipe_sequence(char *cmd_line)
{
	int	i;
	int	len;
	int	flag;

	flag = 0;
	i = -1;
	len = ft_strlen(cmd_line);
	while (++i < len)
	{
		if (is_whites_space(cmd_line[i]))
			continue ;
		if (cmd_line[i] == '|' && flag)
			return (1);
		if (cmd_line[i] == '|')
			flag++;
		if (cmd_line[i] && cmd_line[i + 1] && cmd_line[i + 1])
		{
			flag++;
			i++;
		}
		if (cmd_line[i] != '|')
			flag--;
	}
	return (0);
}

int	ft_check_operator_sequence(char *cmd_line)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(cmd_line);
	while (i < len)
	{
		if (cmd_line[i] == '&' && cmd_line[i + 1] && cmd_line[i + 1] == '&'
			&& cmd_line[i + 2] && cmd_line[i + 2] == '&'
			&& !ft_check_quote(cmd_line, i))
			return (1);
		if (cmd_line[i] == '&' && !is_operator(cmd_line[i - 1])
			&& !is_operator(cmd_line[i + 1]))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_pipe_sequence_error(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (is_whites_space(cmd[i]))
			continue ;
		if (is_pipe(cmd[i]))
		{
			if (cmd[i + 1] && is_pipe(cmd[i + 1]))
				i++;
			while (cmd[++i])
			{
				if (is_whites_space(cmd[i]))
					continue ;
				if (is_pipe(cmd[i]) || is_operator(cmd[i])
					|| is_r_parenthise(cmd[i]))
					return (0);
				break ;
			}
			if (!cmd[i])
				return (0);
		}
	}
	return (1);
}

int	ft_operatore_sequence_error(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (is_whites_space(cmd[i]))
			continue ;
		if (is_operator(cmd[i]) && !ft_check_quote(cmd, i))
		{
			if (cmd[i + 1] && is_operator(cmd[i + 1]))
				i++;
			else
				return (0);
			while (cmd[++i])
			{
				if (is_whites_space(cmd[i]))
					continue ;
				if (is_pipe(cmd[i]) || is_operator(cmd[i])
					|| is_r_parenthise(cmd[i]))
					return (0);
				break ;
			}
		}
	}
	return (1);
}
