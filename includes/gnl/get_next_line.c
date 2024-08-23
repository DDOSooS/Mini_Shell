/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:52:57 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/23 10:10:25 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_new_lines(char *read_lines)
{
	int		i;
	int		j;
	char	*new_lines;

	i = 0;
	while (read_lines[i] && read_lines[i] != '\n')
		i++;
	if (!read_lines[i])
	{
		free(read_lines);
		return (NULL);
	}
	new_lines = (char *)malloc(sizeof(char) * (ft_strlen_gnl(read_lines) - i
				+ 1));
	if (!new_lines)
		return (NULL);
	i++;
	j = 0;
	while (read_lines[i])
		new_lines[j++] = read_lines[i++];
	new_lines[j] = '\0';
	free(read_lines);
	return (new_lines);
}

char	*ft_lines(char *read_lines)
{
	char	*lines;
	int		i;

	i = 0;
	if (!read_lines[i])
		return (NULL);
	while (read_lines[i] && read_lines[i] != '\n')
		i++;
	lines = (char *)malloc(sizeof(char) * i + 2);
	if (!lines)
		return (NULL);
	i = 0;
	while (read_lines[i] && read_lines[i] != '\n')
	{
		lines[i] = read_lines[i];
		i++;
	}
	if (read_lines[i] == '\n')
		lines[i] = read_lines[i];
	lines[i] = '\0';
	return (lines);
}

char	*ft_read_lines(int fd, char *read_lines)
{
	char	*buffer;
	int		r;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	r = 1;
	while (!ft_strchr_gnl(read_lines, '\n') && r != 0)
	{
		r = read(fd, buffer, BUFFER_SIZE);
		if (r == -1)
		{
			free(buffer);
			if (read_lines)
				free(read_lines);
			return (NULL);
		}
		buffer[r] = '\0';
		read_lines = ft_strjoin_gnl(read_lines, buffer);
	}
	free(buffer);
	return (read_lines);
}

char	*get_next_line(int fd)
{
	static char	*read_lines;
	char		*lines;

	if (BUFFER_SIZE <= 0 || fd < 0 || BUFFER_SIZE >= 2147483647)
		return (0);
	read_lines = ft_read_lines(fd, read_lines);
	if (!read_lines)
		return (NULL);
	lines = ft_lines(read_lines);
	read_lines = ft_new_lines(read_lines);
	return (lines);
}
/*
#include <stdio.h>

int	main(void) {
	int o = open("test.txt", O_RDONLY);
	if (o < 0)
		return (-1);
	char *lines;
	int x = 0;
	while (x < 10)
	{
		lines = get_next_line(o);
		printf("%d -> %s", x++, lines);
		free(lines);
	}
	close(o);
	system("leaks a.out");
	return (0);
}
*/
