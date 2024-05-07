/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddos <ddos@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:57:08 by aghergho          #+#    #+#             */
/*   Updated: 2023/12/18 08:42:27 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*edit_line(char *str, int len, char **g_line)
{
	char	*s;

	s = NULL;
	s = str_sub(str, len + 1);
	if (!s)
		return (NULL);
	*g_line = str_sub(str + len + 1, ft_strlen(str + len + 1));
	if (! *g_line)
	{
		free(s);
		free(*g_line);
		return (NULL);
	}
	free (str);
	return (s);
}

char	*str_join(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	j = -1;
	i = 0;
	if (s1)
		i += ft_strlen(s1);
	i += ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * i + 1);
	if (! str)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2 && s2[++j])
		str[i + j] = s2[j];
	str[i + j] = '\0';
	free(s1);
	return (str);
}

char	*get_lin(int fd, char *g_line)
{
	char	*buffer;
	int		b_read;

	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	b_read = read(fd, buffer, BUFFER_SIZE);
	while (b_read > 0)
	{
		buffer[b_read] = '\0';
		g_line = str_join(g_line, buffer);
		if (!g_line || check_end_line(g_line) >= 0)
			break ;
		b_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (!g_line && b_read != 0)
	{
		free(g_line);
		g_line = NULL;
	}
	return (g_line);
}

char	*get_next_line(int fd)
{
	static char	*line;
	char		*str;
	int			e_line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	str = get_lin(fd, line);
	if (!str)
		return (NULL);
	e_line = check_end_line(str);
	if (e_line >= 0 && e_line + 1 != ft_strlen(str))
	{
		str = edit_line(str, e_line, &line);
		if (! str)
			return (NULL);
	}
	else if ((e_line < 0 || ft_strlen(str) == e_line + 1) && line)
		line = NULL;
	return (str);
}
