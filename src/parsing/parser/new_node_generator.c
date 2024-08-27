/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_node_generator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:03:58 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 21:20:25 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	ft_get_redirection(t_redirection **redirection, t_token *token,
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
		if ((token->type_id >= 1 && token->type_id <= 3) || token->type_id == 5)
			break ;
		if (token->type_id >= 6 && token->type_id <= 9)
		{
			ft_add_redirection(redirection, token, inflag, outflag);
			token = token->next;
		}
		token = token->next;
	}
}

t_cmd	*ft_gen_cmd(t_token *tokens)
{
	int		flag;
	t_cmd	*cmd;
	char	**args;

	args = NULL;
	cmd = NULL;
	flag = 0;
	while (tokens && tokens->value)
	{
		if (!tokens->type_id && !flag)
			ft_add_to_cmd(&cmd, tokens->value);
		if (tokens->type_id >= 6 && tokens->type_id <= 9)
			flag = 1;
		if (!tokens->type_id && flag)
			flag = 0;
		tokens = tokens->next;
	}
	astrict_exp(&cmd);
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
			while (tokens && tokens->type_id != 5)
				tokens = tokens->next;
			ft_get_redirection(&new->redirection, tokens->next, 1, 1);
		}
	}
	else
	{
		new->cmd = ft_gen_cmd(tokens);
		ft_get_redirection(&new->redirection, tokens, 1, 1);
	}
	return (new);
}
