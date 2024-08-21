#include "../../../includes/mshell.h"

static int	handle_input_redirection(t_infile *in_file)
{
	char	(*filename);

	int (here_doc_num), (fd);
	here_doc_num = 0;
	while (in_file)
	{
		if (in_file->mode == TOKEN_HERDOC)
		{
			filename = create_heredoc_filename(here_doc_num++);
			fd = open(filename, O_RDONLY);
			free(filename);
		}
		else if (in_file->mode == TOKEN_IN_REDIR)
		{
			fd = open(in_file->filename, O_RDONLY);
			if (fd == -1)
				return (printf("minishell: %s: %s\n", in_file->filename,
						strerror(errno)), -1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		in_file = in_file->next;
	}
	return (0);
}

static int	handle_output_redirection(t_outfile *out_file)
{
	int	fd;

	while (out_file)
	{
		if (out_file->mode == TOKEN_OUT_REDIR)
			fd = open(out_file->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (out_file->mode == TOKEN_APPEND)
			fd = open(out_file->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
		{
			printf("minishell: %s: %s\n", out_file->filename, strerror(errno));
			return (-1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		out_file = out_file->next;
	}
	return (0);
}

int	apply_redirections(t_tnode *root)
{
	if (root == NULL)
		return (0);
	if (root->redirection->in_file)
	{
		if (handle_input_redirection(root->redirection->in_file) == -1)
			return (-1);
	}
	if (root->redirection->out_file)
	{
		if (handle_output_redirection(root->redirection->out_file) == -1)
			return (-1);
	}
	return (0);
}
