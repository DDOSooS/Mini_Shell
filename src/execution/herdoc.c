#include "../../includes/mshell.h"

int heredoc_cheker(char*str, char *filename, int fd)
{
	if (str == NULL)
	{
		write(2, "minishell: warning: here-document delimited by end-of-file\n", 60);
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

char *create_heredoc_filename(int here_doc_num)
{
	char	*heredoc_tempfile;

	heredoc_tempfile = (char *)malloc(ft_strlen("/tmp/heredoc_") + 2);
	ft_strlcpy(heredoc_tempfile, "/tmp/heredoc_", ft_strlen("/tmp/heredoc_") + 2);
	heredoc_tempfile[ft_strlen("/tmp/heredoc_")] = here_doc_num + '0';
	heredoc_tempfile[ft_strlen("/tmp/heredoc_") + 1] = '\0';
	return (heredoc_tempfile);
}

void update_history_from_pipe(int fd, t_history *history)
{
    char buffer[1024];
    int bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
	{
        buffer[bytes_read] = '\0';
        put_tohistory(buffer, history, 1);
    }
}

int write_to_fd(int fd, char *str)
{
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
	return 0;
}

void create_heredoc(char *del, int id, int write_fd)
{
    int fd;
    char (*line), (*filename);

    filename = create_heredoc_filename(id);
    fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1)
	{
        printf("open: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    while (1)
	{
        line = readline(">");
        if (line)
			write_to_fd(write_fd, line);
        if (heredoc_cheker(line, del, fd))
            break;
		if (ft_strchr(line, '$'))
			ft_expand_arg(&line);
		write_to_fd(fd, line);
        if (line)
            free(line);
    }
	free(filename);
    close(fd);
}

int ft_heredoc(t_tnode *root, t_mshell *shell)
{
    t_herdoc *herdoc;
    int pid;
    int status;
    int pipe_fds[2];

    status = 0;
    herdoc = shell->herdocs;
    if (pipe(pipe_fds) == -1)
	{
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);
    pid = fork();
    if (pid == 0)
	{
        close(pipe_fds[0]);
        handle_signals(active_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
        while (herdoc)
		{
            create_heredoc(herdoc->delimiter, herdoc->id, pipe_fds[1]);
            herdoc = herdoc->next;
        }
        close(pipe_fds[1]);
        exit(EXIT_SUCCESS);
    }
	else
	{
        close(pipe_fds[1]);
        waitpid(pid, &status, 0);
        status = get_status(status);
        update_history_from_pipe(pipe_fds[0], shell->history);
        close(pipe_fds[0]);
    }
    handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
    return status;
}
