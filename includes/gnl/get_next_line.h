/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:52:50 by mkartit           #+#    #+#             */
/*   Updated: 2023/11/25 15:59:07 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

char	*ft_strjoin_gnl(char *s1, char *s2);
char	*ft_strchr_gnl(char *s, char c);
size_t	ft_strlen_gnl(char *s);
// char	*ft_strjoin(char *s1, char *s2);
// size_t	ft_strlen(char *s);
// char	*ft_strchr(char *s, char c);
char	*ft_read_lines(int fd, char *get_line);
char	*ft_lines(char *read_lines);
char	*ft_new_lines(char *read_lines);
char	*get_next_line(int fd);

#endif /* GET_NEXT_LINE_H */
