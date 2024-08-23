/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:06:08 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/23 10:05:59 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

// those function are responsible forn parsing the tokenizer
// and generate the command table and also well hanele quote expand cases

/*
	flag is an index to indecate which logical operation
	flag 0-> it's a commamd to be executed
	flag 1-> is pipe (|)
	flag 2-> is pipe (||)
	flag 3-> is pipe (&&)

				| 1
			/   \
			(||)2   |1
			/\    / \
		c10  c20  c10 |
					/ \
					0c1  0c2
*/
/*
	flag 0-> it's a commamd to be executed
	flag is an index to indecate which logical operation
	flag 1-> is pipe (|)
	flag 2-> is pipe (||)
	flag 3-> is pipe (&&)
	flag 4-> parenthisis  (())
*/

void	ft_parser_helper(char **token_value, t_tnode **root, t_tnode **new)
{
	free(*token_value);
	(*token_value) = NULL;
	(*new)->t_parent = *root;
}

void	ft_parse_pipe(t_tnode **root, t_token **tokens)
{
	t_token	*tmp;
	t_tnode	*new;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->value && is_pipe(tmp->value[0]) && !ft_check_pipe(tmp->next))
			break ;
		tmp = tmp->next;
	}
	new = ft_new_tnode(1, *tokens);
	if (!*root)
		*root = new;
	else
	{
		if ((*root)->t_left)
			(*root)->t_right = new;
		else
			(*root)->t_left = new;
	}
	ft_parser_helper(&tmp->value, root, &new);
	ft_parse_ast(&new, tokens);
	ft_parse_ast(&new, &tmp->next);
}

void	ft_parse_or_operator(t_tnode **root, t_token **tokens)
{
	t_token	*tmp;
	t_tnode	*new;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->value && is_pipe(tmp->value[0]) && is_pipe(tmp->value[1])
			&& is_last_operator(tmp->next))
			break ;
		tmp = tmp->next;
	}
	new = ft_new_tnode(2, *tokens);
	if (!*root)
		*root = new;
	else
	{
		if (!(*root)->t_left)
			(*root)->t_left = new;
		else
			(*root)->t_right = new;
	}
	ft_parser_helper(&tmp->value, root, &new);
	ft_parse_ast(&new, tokens);
	ft_parse_ast(&new, &(tmp->next));
}

void	ft_parse_and_operator(t_tnode **root, t_token **tokens)
{
	t_token	*tmp;
	t_tnode	*new;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->value && is_operator(tmp->value[0])
			&& is_last_operator(tmp->next))
			break ;
		tmp = tmp->next;
	}
	new = ft_new_tnode(3, *tokens);
	if (!*root)
		*root = new;
	else
	{
		if (!(*root)->t_left)
			(*root)->t_left = new;
		else
			(*root)->t_right = new;
	}
	ft_parser_helper(&tmp->value, root, &new);
	ft_parse_ast(&new, tokens);
	ft_parse_ast(&new, &(tmp->next));
}

void	ft_parse_ast(t_tnode **root, t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	if (tmp && tmp->value)
	{
		if (ft_check_and_operator(tmp))
			ft_parse_and_operator(root, tokens);
		else if (ft_check_or_operator(tmp))
			ft_parse_or_operator(root, tokens);
		else if (ft_check_pipe(tmp))
			ft_parse_pipe(root, tokens);
		else if (ft_check_parenthises(tmp))
			ft_parse_parenthise(root, tokens);
		else
			ft_parse_cmd(root, tokens);
	}
}
