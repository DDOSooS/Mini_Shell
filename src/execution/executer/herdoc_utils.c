/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:33:11 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/27 18:00:21 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	heredoc_cheker(char *str, char *filename, int fd)
{
	if (str == NULL)
	{
		write(2, "minishell: warning: here-document delimited by end-of-file\n",
			60);
		close(fd);
		return (1);
	}
	if (ft_strcmp(str, filename) == 0)
	{
		free(str);
		close(fd);
		return (1);
	}
	return (0);
}

char	*create_heredoc_filename(int here_doc_num)
{
	char	*heredoc_tempfile;

	heredoc_tempfile = (char *)malloc(ft_strlen("/tmp/heredoc_") + 2);
	ft_strlcpy(heredoc_tempfile, "/tmp/heredoc_", ft_strlen("/tmp/heredoc_")
		+ 2);
	heredoc_tempfile[ft_strlen("/tmp/heredoc_")] = here_doc_num + '0';
	heredoc_tempfile[ft_strlen("/tmp/heredoc_") + 1] = '\0';
	return (heredoc_tempfile);
}

void	update_history_from_pipe(int fd, t_history *history)
{
	char	buffer[1024];
	int		bytes_read;

	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		put_tohistory(buffer, history, 1);
		bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	}
}

int	write_to_fd(int fd, char *str)
{
	if (str)
		write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
	return (0);
}

void	create_heredoc(t_herdoc **herdoc, int write_fd)
{
	char	*line;

	int (fd), (expand_flag);
	expand_flag = ft_check_expand_delimiter((*herdoc)->delimiter);
	ft_trim_delimiter_quotes(&(*herdoc)->delimiter);
	fd = open((*herdoc)->filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		(perror("open"), exit(EXIT_FAILURE));
	while (1)
	{
		line = readline(">");
		if (line)
			write_to_fd(write_fd, line);
		if (heredoc_cheker(line, (*herdoc)->delimiter, fd))
			break ;
		if (expand_flag)
			ft_expand_herdoc_arg(&line);
		write_to_fd(fd, line);
		if (line)
			free(line);
	}
	close(fd);
}
