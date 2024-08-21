/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_generator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:04:16 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/21 13:04:17 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ftAddOutFile(t_outfile **root, t_token *token)
{
	t_outfile	*new;
	t_outfile	*tmp;

	new = malloc(sizeof(t_outfile));
	if (!new)
		return (0);
	new->mode = token->typeId;
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

int	ftAddInFile(t_infile **root, t_token *token)
{
	t_infile	*new;
	t_infile	*tmp;

	new = malloc(sizeof(t_infile));
	if (!new)
		return (0);
	new->mode = token->typeId;
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

int	ftAddRedirection(t_redirection **root, t_token *token, int inredirection,
		int out_redirection)
{
	if ((token->typeId == 7 || token->typeId == 8) && inredirection)
		ftAddInFile(&(*root)->in_file, token);
	if ((token->typeId == 6 || token->typeId == 9) && out_redirection)
		ftAddOutFile(&(*root)->out_file, token);
	return (1);
}
