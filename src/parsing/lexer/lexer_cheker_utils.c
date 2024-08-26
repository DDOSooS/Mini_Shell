/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cheker_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:02:08 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/26 10:06:11 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	out_redirection(char c)
{
	if (c == '>')
		return (1);
	return (0);
}

int	is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '&')
		return (1);
	return (0);
}

int	is_r_parenthise(char c)
{
	if (c == ')')
		return (1);
	return (0);
}

int	is_l_parenthise(char c)
{
	if (c == '(')
		return (1);
	return (0);
}
