/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:04:10 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 19:05:07 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	ft_parse_cmd(t_tnode **root, t_token **tokens)
{
	t_tnode	*new;

	new = ft_new_tnode(0, *tokens);
	if (!*root)
		*root = new;
	else
	{
		new->t_parent = *root;
		if (!(*root)->t_left)
			(*root)->t_left = new;
		else
			(*root)->t_right = new;
	}
}

void	ft_parse_parenthise(t_tnode **root, t_token **tokens)
{
	t_token	*tmp;
	t_tnode	*new;
	t_token	*tparenthise;
	t_token	*tmp1;

	tmp = *tokens;
	while (tmp && tmp->value && !is_l_parenthise(tmp->value[0]))
		tmp = tmp->next;
	free(tmp->value);
	free(tmp->next->next->value);
	tmp->value = NULL;
	tmp->next->next->value = NULL;
	new = ft_new_tnode(4, *tokens);
	ft_insert_node(root, &new);
	tparenthise = ft_tokinizer(tmp->next->value);
	tmp1 = tparenthise;
	while (tmp1->next)
		tmp1 = tmp1->next;
	tmp1->next = tmp->next->next;
	free(tmp->next->value);
	free(tmp->next);
	tmp->next = tparenthise;
	ft_parse_ast(&new, &tparenthise);
}
