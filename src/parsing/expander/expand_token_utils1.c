/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:51:43 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 10:39:28 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	is_pid_token(char *token, int i)
{
	return (is_dollar_sign(token[i]) && token[i + 1]
		&& is_dollar_sign(token[i + 1]));
}

int	is_expanded_token(char *token, int i)
{
	return (is_dollar_sign(token[i]) && token[i + 1]
		&& (ft_isalnum(token[i + 1]) || token[i + 1] == '_'));
}

int	check_middle_white_space(char *token)
{
	int	i;

	i = -1;
	if (!*token)
		return (1);
	while (token[++i] && is_white_space(token[i]))
		;
	while (token[i])
	{
		if (is_white_space(token[i]))
		{
			while (token[i] && is_white_space(token[i]))
				i++;
			if (!token[i])
				return (0);
			return (1);
		}
		i++;
	}
	return (0);
}
