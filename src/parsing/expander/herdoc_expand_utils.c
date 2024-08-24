/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 10:57:34 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/22 12:09:50 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	ft_free_herdoc(t_herdoc **herdocs)
{
	t_herdoc	*tmp;
	t_herdoc	*tmp2;

	if (!herdocs || !*herdocs)
		return ;
	tmp = *herdocs;
	while (tmp)
	{
		tmp2 = tmp->next;
		if (tmp)
		{
			free(tmp->delimiter);
			free(tmp->filename);
			free(tmp);
		}
		tmp = tmp2;
	}
	tmp = NULL;
}

int	ft_count_expanded_len(char *delimimiter)
{
	char	quote;

	int (counter), i;
	i = -1;
	counter = 0;
	while (delimimiter[++i])
	{
		if (is_quote(delimimiter[i]) && delimimiter[i + 1]
			&& delimimiter[i] == delimimiter[i + 1])
		{
			i++;
			continue ;
		}
		if (is_quote(delimimiter[i]))
		{
			quote = delimimiter[i];
			while (delimimiter[++i] && delimimiter[i] != quote)
				counter++;
		}
		else
			counter++;
	}
	return (counter);
}

void	ft_gen_expanded_delimiter(char **new_del, char *del)
{
	int		i;
	char	quote;

	i = -1;
	while (del[++i])
	{
		if (is_quote(del[i]) && del[i + 1] && del[i] == del[i + 1])
		{
			i++;
			continue ;
		}
		if (is_quote(del[i]))
		{
			quote = del[i];
			i++;
			while (del[i] && del[i] != quote)
			{
				ft_strcat_char(*new_del, del[i]);
				i++;
			}
		}
		else
			ft_strcat_char(*new_del, del[i]);
	}
}
