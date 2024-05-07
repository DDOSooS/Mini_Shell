/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:31:26 by aghergho          #+#    #+#             */
/*   Updated: 2023/12/11 16:31:29 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*str_sub(char *str, int len)
{
	char	*s;
	int		i;

	if (!len)
		len++;
	s = (char *) malloc(sizeof(char) * len + 1);
	if (!s)
		return (NULL);
	i = 0;
	while (i < len && str[i])
	{
		s[i] = str[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

int	ft_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

//checking the end_of_line
int	check_end_line(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	if (str[0] == '\n')
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}
