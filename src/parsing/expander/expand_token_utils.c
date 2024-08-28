/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:26:08 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 22:54:00 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	is_question_symbol(char c)
{
	return (c == '?');
}

int	is_symbol(char c)
{
	return (ft_isalnum(c) || is_dollar_sign(c)
		|| is_question_symbol(c) || c == '_');
}

void	ft_reset_token(t_token **token, t_token **root)
{
	if (*token == *root)
		*root = (*token)->next;
	if (!(*token)->previous && (*token)->next)
	{
		(*token)->next->previous = NULL;
	}
	else if ((*token)->previous && (*token)->next)
	{
		(*token)->previous->next = (*token)->next;
		(*token)->next->previous = (*token)->previous;
	}
	else if ((*token)->previous && !(*token)->next)
		(*token)->previous->next = NULL;
}

void	ft_expand_token_helper(int *flag, t_token **token, int type)
{
	if ((*token)->value && ft_check_dollar((*token)->value))
	{
		*flag = 1;
		if (ft_check_unquoted_dollar((*token)->value))
			(*token)->is_exported = 1;
	}
	ft_expand_arg(&(*token)->value, type);
}

void	ft_delete_token(t_token **token, t_token **root)
{
	t_token	*sd_tmp;

	sd_tmp = NULL;
	sd_tmp = (*token)->next;
	ft_reset_token(token, root);
	if ((*token)->value)
		free((*token)->value);
	free((*token));
	(*token) = NULL;
	(*token) = sd_tmp;
}
