/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:50:16 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 22:02:18 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

// world[world]* [|world [world]*]*
// [[>world][<world][>&world][>>world][>>&world]]* [&&|| ()] [&]
// ls -l <<< file should work | cat makefile >| d

int	ft_check_right_parenthise(char *cmd)
{
	int	i;
	int	redirection_found;

	redirection_found = 0;
	i = -1;
	while (cmd[++i] && is_r_parenthise(cmd[i]))
		;
	while (cmd[i] && !is_pipe(cmd[i]) && !is_operator(cmd[i]))
	{
		if (is_white_space(cmd[i]) || is_r_parenthise(cmd[i])
			|| is_redirection(cmd[i]))
		{
			ft_check_redirection(cmd[i], &redirection_found);
			i++;
			continue ;
		}
		if (redirection_found)
		{
			i += ft_count_file_name(cmd, i);
			redirection_found = 0;
		}
		else
			return (0);
	}
	return (1);
}

int	ft_check_parenthise_sequence(char *cmd_line)
{
	int	i;

	i = -1;
	while (cmd_line[++i])
	{
		if (is_l_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
		{
			if (!ft_check_left_parenthise(cmd_line, i))
				return (0);
		}
		if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
		{
			if (!ft_check_right_parenthise(&cmd_line[i + 1]))
				return (0);
		}
	}
	return (1);
}

int	ft_check_parenthise_error(char *cmd_line)
{
	if (!ft_check_closed_parenthise(cmd_line)
		|| !ft_check_empty_parenthise(cmd_line))
		return (0);
	if (!ft_check_parenthise_sequence(cmd_line))
		return (ft_putstr_fd("Syntax error near unexpected token `('\n", 2), 0);
	return (1);
}

int	ft_check_empty_command(char *cmd_line)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(cmd_line);
	while (i < len && is_whites_space(cmd_line[i]))
		i++;
	if (i == len)
		return (1);
	return (0);
}

int	ft_check_syntax(char *cmd_line)
{
	if (!ft_strlen(cmd_line) || ft_check_empty_command(cmd_line))
		return (1);
	if (ft_check_operation_error(cmd_line) || ft_check_quote_error(cmd_line)
		|| !ft_check_redirection_error(cmd_line))
	{
		g_mshell.exit_value = 2;
		return (0);
	}
	if (!ft_check_parenthise_error(cmd_line))
	{
		g_mshell.exit_value = 2;
		return (0);
	}
	return (1);
}
