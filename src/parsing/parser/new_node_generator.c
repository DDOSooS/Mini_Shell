/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_node_generator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:03:58 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/21 13:03:59 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	ftGetRedirection(t_redirection **redirection, t_token *token,
		int inflag, int outflag)
{
	if (!*redirection)
	{
		*redirection = malloc(sizeof(t_redirection));
		(*redirection)->in_file = NULL;
		(*redirection)->out_file = NULL;
	}
	while (token)
	{
		if ((token->typeId >= 1 && token->typeId <= 3) || token->typeId == 5)
			break ;
		if (token->typeId >= 6 && token->typeId <= 9)
		{
			ftAddRedirection(redirection, token, inflag, outflag);
			token = token->next;
		}
		token = token->next;
	}
}

t_cmd	*ftGenCmd(t_token *tokens)
{
	int		flag;
	t_cmd	*cmd;

	cmd = NULL;
	flag = 0;
	while (tokens && tokens->value)
	{
		if (!tokens->typeId && !flag)
		{
			if (tokens->value[0] && is_double_quote(tokens->value[0])
				&& is_double_quote(tokens->value[ft_strlen(tokens->value) - 1]))
				ft_add_to_cmd(&cmd, tokens->value);
			else if (!ftAddCmd(&cmd, tokens->value))
				return (NULL);
		}
		if (tokens->typeId >= 6 && tokens->typeId <= 9)
			flag = 1;
		if (!tokens->typeId && flag)
			flag = 0;
		tokens = tokens->next;
	}
	return (cmd);
}

void	ft_insert_node(t_tnode **root, t_tnode **new)
{
	if (!*root)
		*root = *new;
	else
	{
		if ((*root)->t_left)
			(*root)->t_right = *new;
		else
			(*root)->t_left = *new;
	}
}
t_tnode	*ft_gen_new_node(int n_type)
{
	t_tnode	*new;

	new = malloc(sizeof(t_tnode));
	if (!new)
		return (NULL);
	new->node_type = n_type;
	new->t_right = NULL;
	new->t_left = NULL;
	new->t_parent = NULL;
	new->cmd = NULL;
	new->redirection = NULL;
	return (new);
}

t_tnode	*ft_new_tnode(int n_type, t_token *tokens)
{
	t_tnode	*new;

	new = ft_gen_new_node(n_type);
	if (!new)
		return (NULL);
	if (n_type)
	{
		if (n_type == 4 && is_parenthise_redirection(tokens))
		{
			while (tokens && tokens->typeId != 5)
				tokens = tokens->next;
			ftGetRedirection(&new->redirection, tokens->next, 1, 1);
		}
	}
	else
	{
		new->cmd = ftGenCmd(tokens);
		ftGetRedirection(&new->redirection, tokens, 1, 1);
	}
	return (new);
}
