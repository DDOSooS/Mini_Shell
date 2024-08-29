/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_checker.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:49:43 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 22:55:24 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

/*
	@return (int)
	@Description this function return the type
	id of a token that well be used in the parser
*/

int	ft_get_token_id(char *token)
{
	if (is_operator(token[0]) && g_mshell.flag)
		return (3);
	if (is_or_operator(token) && g_mshell.flag)
		return (2);
	else if (is_pipe(token[0]) && g_mshell.flag)
		return (1);
	else if (is_l_parenthise(token[0]) && g_mshell.flag)
		return (4);
	else if (is_r_parenthise(token[0]) && g_mshell.flag)
		return (5);
	else if (is_append(token))
		return (6);
	else if (is_herdoc(token))
		return (7);
	else if (in_redirection(token[0]))
		return (8);
	else if (out_redirection(token[0]))
		return (9);
	return (0);
}

int	ft_check_parenthisis_spaces(char *cmd, int index)
{
	int	i;

	i = index;
	while (i >= 0 && is_whites_space(cmd[i]))
		i--;
	if (i >= 0 && is_parenthise(cmd[i]))
		return (0);
	if (!is_closed_parenthise(cmd, i))
		return (1);
	return (0);
}

int	ft_check_opened_token(char *cmd, int len)
{
	if (!ft_check_quote(cmd, len) && is_closed_parenthise(cmd, len))
		return (1);
	return (0);
}

int	ft_check_l_parenthise(char *cmd, int i)
{
	while (cmd[i])
	{
		if (is_whites_space(cmd[i]) && !ft_check_quote(cmd, i))
		{
			i++;
			continue ;
		}
		if (is_l_parenthise(cmd[i]) && !ft_check_quote(cmd, i))
			return (1);
		i++;
	}
	return (0);
}

int	ft_check_r_parenthise(char *cmd, int i)
{
	if (!cmd[i])
		return (0);
	while (cmd[i])
	{
		if (is_whites_space(cmd[i]) && !ft_check_quote(cmd, i))
		{
			i++;
			continue ;
		}
		if (is_r_parenthise(cmd[i]) && !ft_check_quote(cmd, i))
			return (1);
		i++;
	}
	return (0);
}
