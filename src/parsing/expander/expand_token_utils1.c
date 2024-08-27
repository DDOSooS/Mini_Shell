/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:51:43 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 18:55:03 by aghergho         ###   ########.fr       */
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
	return (is_dollar_sign(token[i])
		&& token[i + 1] && ft_isalnum(token[i + 1]));
}
