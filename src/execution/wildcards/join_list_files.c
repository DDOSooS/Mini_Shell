/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_list_files.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:40 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:34:41 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

char	*consume_extra(char *reg, char c)
{
	while (*reg == c && *(reg + 1) == c)
		reg += 1;
	return (reg);
}

static int	list_size(char **list)
{
	int	i;

	i = 0;
	while (list[i])
		i++;
	return (i);
}

static void	move_pointer(char **dest, char **src, int *b_i)
{
	while (*src)
	{
		dest[*b_i] = *src;
		*b_i += 1;
		src++;
	}
}

char	**list_join(char **list_one, char **list_two, int i)
{
	int		b_i;
	char	**buff;

	if (list_one == 0 || list_two == 0)
		return (0);
	buff = ft_calloc(list_size(list_one) + list_size(list_two) + 1,
			sizeof(char *));
	if (buff == 0)
		return (0);
	b_i = 0;
	while (*list_one)
	{
		if (b_i == i)
			move_pointer(buff, list_two, &b_i);
		else
		{
			buff[b_i] = *list_one;
			b_i++;
		}
		list_one++;
	}
	return (buff);
}

int	find_char(char *str, char c)
{
	int	i;

	if (str == 0)
		return (-1);
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i += 1;
	}
	return (-1);
}
