/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:56:10 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/22 13:47:25 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_check_operation_error(char *cmd)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(cmd);
	while (i < len && (cmd[i] == ' ' || cmd[i] == '\t'))
		i++;
	if (cmd[i] && (cmd[i] == '|' || cmd[len - 1] == '|'))
		return (ft_putstr_fd("parse error near `|'\n", 2), 1);
	if (!ft_pipe_sequence_error(cmd))
		return (ft_putstr_fd("parse error near `|'\n", 2), 1);
	if (cmd[i] && (cmd[i] == '&' || cmd[len - 1] == '&'))
		return (ft_putstr_fd("parse error near `&'\n", 2), 1);
	if (!ft_operatore_sequence_error(cmd))
		return (ft_putstr_fd("parse error near `&'\n", 2), 1);
	return (0);
}

int	ft_check_quote_error(char *cmd_line)
{
	int	d_quote;
	int	s_quote;
	int	i;
	int	len;

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
		if (cmd_line[i] == 39 && !d_quote && !s_quote)
			s_quote = 1;
		else if (cmd_line[i] == 39 && s_quote)
			s_quote = 0;
	}
	if (d_quote || s_quote)
		return (ft_putstr_fd(";( Syntax error please verify your quotes\n"
				, 2), 1);
	return (0);
}

int	ft_check_redirection_sequence(char *cmd_line)
{
	int	i;
	int	len;

	i = -1;
	len = ft_strlen(cmd_line);
	while (++i < len)
	{
		if (cmd_line[i] == '>' && cmd_line[i + 1] && cmd_line[i + 1] == '>'
			&& cmd_line[i + 2] && cmd_line[i + 2] == '>'
			&& !ft_check_quote(cmd_line, i))
		{
			ft_putstr_fd("parse error near `>'\n", 2);
			return (1);
		}
		if (cmd_line[i] == '<' && cmd_line[i + 1] && cmd_line[i + 1] == '<'
			&& cmd_line[i + 2] && cmd_line[i + 2] == '<'
			&& !ft_check_quote(cmd_line, i))
		{
			ft_putstr_fd("parse error near `<'\n", 2);
			return (1);
		}
	}
	return (0);
}

int	ft_check_file_name(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i] && is_whites_space(cmd[i]))
		;
	if (!cmd[i])
		return (ft_putstr_fd("parse error near `/n'\n", 2), 0);
	if (is_operator(cmd[i]) && cmd[i + 1] && is_operator(cmd[i + 1]))
		return (ft_putstr_fd("parse error near `&&'\n", 2), 0);
	if (is_operator(cmd[i]))
		return (ft_putstr_fd("parse error near `&'\n", 2), 0);
	else if (is_pipe(cmd[i]) && cmd[i + 1] && is_pipe(cmd[i + 1]))
		return (ft_putstr_fd("parse error near `||'\n", 2), 0);
	else if (is_pipe(cmd[i]))
		return (ft_putstr_fd("parse error near `|'\n", 2), 0);
	else if (in_redirection(cmd[i]))
		return (ft_putstr_fd("parse error near `<'\n", 2), 0);
	else if (out_redirection(cmd[i]))
		return (ft_putstr_fd("parse error near `>'\n", 2), 0);
	else if (is_r_parenthise(cmd[i]) || is_r_parenthise(cmd[i]))
		return (ft_putstr_fd("parse error near `)'\n", 2), 0);
	return (1);
}

int	ft_check_in_redirection(char *cmd_line, int len)
{
	int	i;

	i = -1;
	while (++i < len)
	{
		if (in_redirection(cmd_line[i]) && !ft_check_quote(cmd_line, i))
		{
			if (cmd_line[i + 1] && in_redirection(cmd_line[i + 1])
				&& !ft_check_quote(cmd_line, i))
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
