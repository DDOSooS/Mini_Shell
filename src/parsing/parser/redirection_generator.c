/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_generator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:04:16 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 10:42:41 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	check_quote_file(char *filename)
{
	if (filename)
	{
		if (is_quote(filename[0])
			&& filename[0] == filename[ft_strlen(filename) - 1])
			return (1);
	}
	return (0);
}

int	ft_add_out_file(t_outfile **root, t_token *token)
{
	t_outfile	*new;
	t_outfile	*tmp;

	new = malloc(sizeof(t_outfile));
	if (!new)
		return (0);
	new->mode = token->type_id;
	if (check_quote_file(token->next->value))
		new->filename = ft_substr(token->next->value,
				1, ft_strlen(token->next->value) - 2);
	else
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
	if (check_quote_file(token->next->value))
		new->filename = ft_substr(token->next->value,
				1, ft_strlen(token->next->value) - 2);
	else
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
