/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_expand_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 10:57:34 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 18:57:50 by aghergho         ###   ########.fr       */
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

void	ft_count_delimiter_helper(int *counter, char *del, int *i, char *quote)
{
	if (is_quote(del[(*i)]))
	{
		*quote = del[(*i)];
		(*i)++;
		while (del[(*i)] && del[(*i)] != *quote)
		{
			(*counter)++;
			(*i)++;
		}
	}
	else
		(*counter)++;
}

int	ft_count_expanded_len(char *delimiter)
{
	char	quote;
	int		counter;
	int		i;

	i = -1;
	counter = 0;
	while (delimiter[++i])
	{
		if (is_quote(delimiter[i]) && delimiter[i + 1]
			&& delimiter[i] == delimiter[i + 1])
		{
			i++;
			continue ;
		}
		if (is_dollar_sign(delimiter[i]) && delimiter[i + 1]
			&& is_quote(delimiter[i + 1]) && !ft_check_quote(delimiter, i + 1))
		{
			if (i - 1 < 0)
				continue ;
			if (i - 1 >= 0 && !is_dollar_sign(delimiter[i - 1]))
				continue ;
		}
		ft_count_delimiter_helper(&counter, delimiter, &i, &quote);
	}
	return (counter);
}

void	ft_gen_delimiter_helper(char **new_del, char *del, int *i, char *quote)
{
	if (is_quote(del[(*i)]))
	{
		*quote = del[(*i)];
		(*i)++;
		while (del[(*i)] && del[(*i)] != *quote)
		{
			ft_strcat_char(*new_del, del[(*i)]);
			(*i)++;
		}
	}
	else
		ft_strcat_char(*new_del, del[(*i)]);
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
		if (is_dollar_sign(del[i]) && del[i + 1] && is_quote(del[i + 1])
			&& !ft_check_quote(del, i + 1))
		{
			if (i - 1 < 0)
				continue ;
			if (i - 1 >= 0 && !is_dollar_sign(del[i - 1]))
				continue ;
		}
		ft_gen_delimiter_helper(new_del, del, &i, &quote);
	}
}
