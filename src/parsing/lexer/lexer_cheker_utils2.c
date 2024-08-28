/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cheker_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:30:59 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 17:41:42 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	is_logical_operator(char *cmd)
{
	if (cmd[0] && cmd[1] && (is_operator(cmd[0]) || (is_pipe(cmd[0])
				&& is_pipe(cmd[1]))))
		return (1);
	return (0);
}

int	ft_check_white_spaces(char *cmd)
{
	int	i;

	if (!cmd)
		return (0);
	i = 0;
	while (cmd[i] && is_white_space(cmd[i]))
		i++;
	while (cmd[i] && cmd[i + 1])
	{
		if (is_white_space(cmd[i]) && !is_white_space(cmd[i + 1]))
			return (1);
		i++;
	}
	return (0);
}

int	is_exist_quote(char *token)
{
	int	i;

	i = -1;
	while (token[++i])
		if (is_quote(token[i]))
			return (1);
	return (0);
}

int	is_herdoc(char *cmd)
{
	if (cmd[0] && cmd[1] && in_redirection(cmd[0]) && in_redirection(cmd[1]))
		return (1);
	return (0);
}

int	is_doubled_token(char *cmd)
{
	if (is_herdoc(cmd) || is_append(cmd) || is_logical_operator(cmd))
		return (1);
	return (0);
}
