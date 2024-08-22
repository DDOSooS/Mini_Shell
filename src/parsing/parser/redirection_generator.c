/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_generator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:04:16 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/22 11:44:38 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_add_out_file(t_outfile **root, t_token *token)
{
	t_outfile	*new;
	t_outfile	*tmp;

	new = malloc(sizeof(t_outfile));
	if (!new)
		return (0);
	new->mode = token->type_id;
	new->filename = ft_strdup(token->next->value);
	new->next = NULL;
	if (!*root)
	{
		*root = new;
		return (1);
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (1);
}

int	ft_add_in_file(t_infile **root, t_token *token)
{
	t_infile	*new;
	t_infile	*tmp;

	new = malloc(sizeof(t_infile));
	if (!new)
		return (0);
	new->mode = token->type_id;
	new->filename = ft_strdup(token->next->value);
	new->next = NULL;
	if (!*root)
	{
		*root = new;
		return (1);
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (1);
}

int	ft_add_redirection(t_redirection **root, t_token *token, int inredirection,
		int out_redirection)
{
	if ((token->type_id == 7 || token->type_id == 8) && inredirection)
		ft_add_in_file(&(*root)->in_file, token);
	if ((token->type_id == 6 || token->type_id == 9) && out_redirection)
		ft_add_out_file(&(*root)->out_file, token);
	return (1);
}
