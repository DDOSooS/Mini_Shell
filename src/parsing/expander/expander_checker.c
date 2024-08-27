/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:18:54 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/22 12:02:16 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	check_dollar_sign_quoted(char *token, int index)
{
	return (is_dollar_sign(token[index]) && is_quote(token[index + 1])
		&& !is_dollar_sign(token[index - 1]) && !ft_check_quote(token, index
			+ 1));
}

int	check_closed_quote_sequence(char *token, int index)
{
	return (is_quote(token[index]) && token[index + 1]
		&& token[index] == token[index + 1] && !ft_check_quote(token, index
			+ 2));
}

int	ft_check_dollar(char *token)
{
	int	i;

	i = -1;
	while (token[++i])
	{
		if (token[i] == '$' && token[i + 1])
			return (1);
	}
	return (0);
}

int	ft_check_expand(char *tmp)
{
	if (is_exist_quote(tmp) || ft_check_dollar(tmp))
		return (1);
	return (0);
}

int	ft_check_expand_delimiter(char *delimiter)
{
	int	checker;
	int	i;

	checker = 0;
	i = -1;
	if (delimiter && !delimiter[0])
		return (0);
	while (delimiter[++i])
	{
		if (is_quote(delimiter[i]))
			return (0);
	}
	return (1);
}
