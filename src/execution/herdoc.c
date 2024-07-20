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
	char *heredoc_tempfile;
	heredoc_tempfile = (char *)malloc(ft_strlen("/tmp/heredoc_") + 2);
	ft_strlcpy(heredoc_tempfile, "/tmp/heredoc_", ft_strlen("/tmp/heredoc_") + 2);
	heredoc_tempfile = ft_strjoin(heredoc_tempfile, ft_itoa(here_doc_num));
	return (heredoc_tempfile);
}

void create_heredoc(char *del, int id)
{
	int fd;
	char *line;
	char *filename;


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
		if (heredoc_cheker(line, del, fd))
			break;
		//TODO: Expand the line
		if (ft_strchr(line, '$'))
			ft_expand_arg(&line);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		
		//TODO: Edit the history to work on herdocs
		//TODO: put_tohistory(line, g_mshell.history, 0);
		if (line)
			free(line);
	}
	close(fd);
}

int ft_heredoc(t_tnode *root, t_mshell *shell)
{
	t_herdoc *herdoc;
	int pid;
	int status;

	status = 0;
	herdoc = shell->herdocs;
	pid = fork();
	if (pid == 0)
	{
		while (herdoc)
		{
			create_heredoc(herdoc->delimiter, herdoc->id);
			herdoc = herdoc->next;
		}
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	return (get_status(status));
}
