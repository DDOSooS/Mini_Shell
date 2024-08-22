/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 01:15:21 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/22 12:01:19 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_expand_token(t_token **tokens)
{
	t_token	*tmp;
	int		flag;

	tmp = *tokens;
	while (tmp)
	{
		flag = 0;
		if (!tmp->previous || (tmp->previous->type_id != 7))
		{
			if (tmp->value && ft_check_dollar(tmp->value))
			{
				flag = 1;
				tmp->is_exported = 1;
			}
			ft_expand_arg(&tmp->value);
		}
		if (!tmp->value[0] && flag && tmp->previous
			&& (tmp->previous->type_id == 6 || tmp->previous->type_id == 8
				|| tmp->previous->type_id == 9))
			return (ft_putstr_fd("ambiguous redirect\n", 2), 0);
		tmp = tmp->next;
	}
	return (1);
}

void	ft_expand_exported_tokens(t_token **tokens, t_token **curr_token)
{
	t_token	*t_tmp;
	t_token	*next_tmp;
	t_token	*last_t_tmp;

	t_tmp = ft_tokinizer((*curr_token)->value);
	ft_expand_tokens(&t_tmp);
	if ((*curr_token)->previous)
		(*curr_token)->previous->next = t_tmp;
	else
		*tokens = t_tmp;
	next_tmp = (*curr_token)->next;
	free((*curr_token)->value);
	free((*curr_token));
	last_t_tmp = t_tmp;
	while (last_t_tmp->next)
		last_t_tmp = last_t_tmp->next;
	last_t_tmp->next = next_tmp;
	(*curr_token) = next_tmp;
}

void	ft_handle_export_expand(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->is_exported && ft_check_white_spaces(tmp->value))
		{
			ft_expand_exported_tokens(tokens, &tmp);
		}
		else
			tmp = tmp->next;
	}
}

void	ft_expand_parenthisis(t_token **tokens)
{
	t_token	*temp_token;
	t_token	*temp;

	temp_token = *tokens;
	while (temp_token)
	{
		if ((temp_token->type_id == 4 || temp_token->type_id == 5)
			&& (temp_token->next
				&& temp_token->next->type_id == temp_token->type_id))
		{
			temp = temp_token->next;
			if (temp_token->previous)
				temp_token->previous->next = temp;
			temp->previous = temp_token->previous;
			if (temp_token == *tokens)
				*tokens = temp;
			free(temp_token->value);
			free(temp_token);
			temp_token = temp;
		}
		else
			temp_token = temp_token->next;
	}
}

// ft_expand_quotes(tokens);

int	ft_expand_tokens(t_token **tokens)
{
	ft_expand_parenthisis(tokens);
	if (!ft_expand_token(tokens))
		return (0);
	ft_handle_export_expand(tokens);
	return (1);
}