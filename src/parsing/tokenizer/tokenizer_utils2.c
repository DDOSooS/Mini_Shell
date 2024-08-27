/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:31:01 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 18:34:42 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int check_parenthise_token(char *token, int index)
{
	if (is_l_parenthise(token[index]))
		return (check_left_p_token(token, index));
	return (check_right_p_token(token, index));
}
