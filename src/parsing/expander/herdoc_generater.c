/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_generater.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 10:57:21 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/24 10:36:40 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

char	*ft_get_delimiter(char *delimiter)
{
	int		len;
	char	*new_delimiter;

	len = ft_count_expanded_len(delimiter);
	new_delimiter = malloc(sizeof(char) * len + 1);
	if (!new_delimiter)
		return (NULL);
	new_delimiter[0] = '\0';
	ft_gen_expanded_delimiter(&new_delimiter, delimiter);
	return (new_delimiter);
}

void	ft_trim_delimiter_quotes(char **delimiter)
{
	char	*trimed_delimter;

	trimed_delimter = NULL;
	if (is_exist_quote(*delimiter))
	{
		trimed_delimter = ft_get_delimiter(*delimiter);
		free(*delimiter);
		*delimiter = trimed_delimter;
	}
}

t_herdoc	*ft_new_herdoc(char *delimiter)
{
	t_herdoc	*new;

	new = malloc(sizeof(t_herdoc));
	if (!new)
		return (NULL);
	new->delimiter = ft_strdup(delimiter);
	new->next = NULL;
	return (new);
}

int	ft_add_herdoc(t_herdoc **root, char *del)
{
	t_herdoc	*new;
	t_herdoc	*tmp;

	new = ft_new_herdoc(del);
	if (!new)
		return (0);
	if (!*root)
	{
		new->id = 0;
		new->filename = create_heredoc_filename(new->id);
		*root = new;
		return (1);
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->id = tmp->id + 1;
	new->filename = create_heredoc_filename(new->id);
	return (1);
}

t_herdoc	*ft_gen_herdocs(t_token *tokens)
{
	t_herdoc	*herdoc;

	herdoc = NULL;
	while (tokens)
	{
		if (tokens->type_id == 7)
		{
			if (!ft_add_herdoc(&herdoc, tokens->next->value))
			{
				ft_free_herdoc(&herdoc);
				return (NULL);
			}
		}
		tokens = tokens->next;
	}
	return (herdoc);
}
