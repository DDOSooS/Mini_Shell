/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cheker_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:04:03 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 16:54:38 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_check_pipe(t_token *token)
{
	while (token && token->value)
	{
		if (is_pipe(token->value[0]) && token->type_id == 1)
			return (1);
		token = token->next;
	}
	return (0);
}

int	ft_check_wild_card(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '*')
			return (1);
	}
	return (0);
}

int	is_parenthise_redirection(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type_id == 5)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

int	check_unclosed_quote(char *token)
{
	int	status;
	int	i;

	i = -1;
	status = 1;
	while (token[++i] && ft_check_quote(token, i + 1))
		;
	if (token[i] && ft_check_quote(token, i))
		status = 0;
	return (status);
}
