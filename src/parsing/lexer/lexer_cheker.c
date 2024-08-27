/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cheker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:55:23 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/26 23:09:39 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	is_dollar_sign(char c)
{
	if (c == '$')
		return (1);
	return (0);
}

int	is_or_operator(char *token)
{
	if (is_pipe(token[0]) && token[1] && is_pipe(token[1]))
		return (1);
	return (0);
}

int	is_tokens(char c)
{
	if (is_redirection(c) || is_pipe(c) || is_operator(c))
		return (1);
	return (0);
}

int	is_parenthise(char c)
{
	if (is_l_parenthise(c) || is_r_parenthise(c))
		return (1);
	return (0);
}

int	in_redirection(char c)
{
	if (c == '<')
		return (1);
	return (0);
}
