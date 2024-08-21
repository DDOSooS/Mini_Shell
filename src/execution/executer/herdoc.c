#include "../../../includes/mshell.h"

static int	run_heredoc_child(t_tnode *root, t_mshell *shell, int pipe_fds[2])
{
	t_herdoc	*herdoc;

	herdoc = shell->herdocs;
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

static int	run_heredoc_parent(int pid, int pipe_fds[2], t_mshell *shell)
{
	int	status;

	status = 0;
	close(pipe_fds[1]);
	waitpid(pid, &status, 0);
	status = get_status(status);
	update_history_from_pipe(pipe_fds[0], shell->history);
	close(pipe_fds[0]);
	return (status);
}

int	ft_heredoc(t_tnode *root, t_mshell *shell)
{
	t_herdoc	*herdoc;

	int (pid), (status), (pipe_fds[2]);
	status = 0;
	herdoc = shell->herdocs;
	if (pipe(pipe_fds) == -1)
		(perror("pipe"), exit(EXIT_FAILURE));
	handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);
	pid = fork();
	if (pid == 0)
		run_heredoc_child(root, shell, pipe_fds);
	else
		status = run_heredoc_parent(pid, pipe_fds, shell);
	handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
	return (status);
}
