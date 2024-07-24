#include "../../includes/mshell.h"

static int handle_input_redirection(t_infile *in_file, t_mshell *shell)
{
	int here_doc_num;
	int fd;
	char *filename;

	here_doc_num = 0;
    while (in_file)
	{
        if (in_file->mode == 7)
		{
			filename = create_heredoc_filename(here_doc_num);
			fd = open(filename, O_RDONLY);
			free(filename);
			here_doc_num++;
		}
		else if (in_file->mode == 8) 
		{
            fd = open(in_file->filename, O_RDONLY);
            if (fd == -1)
			{
				printf("minishell: %s: %s\n", in_file->filename, strerror(errno));
				return (-1);
            }
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
        in_file = in_file->next;
    }
	return (0);
}

static int handle_output_redirection(t_outfile *out_file, t_mshell *shell)
{
	int fd;

    while (out_file)
	{
		if (out_file->mode == 9)
			fd = open(out_file->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (out_file->mode == 6)
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
int apply_redirections(t_tnode *root, t_mshell *shell)
{
	if (root == NULL)
		return (0);
	if (root->redirection->in_file)
	{
		if (handle_input_redirection(root->redirection->in_file, shell) == -1)
			return (-1);
	}
	if (root->redirection->out_file)
	{
		if (handle_output_redirection(root->redirection->out_file, shell) == -1)
			return (-1);
	}
	return (0);
}