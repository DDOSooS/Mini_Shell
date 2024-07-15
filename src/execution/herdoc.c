#include "../../includes/mshell.h"

int heredoc_cheker(char*str, char *filename, int fd)
{
	if (str == NULL)
	{
		write(2, "minishell: warning: here-document delimited by end-of-file\n", 60);
		close(fd);
		return (1);
	}
	if (ft_strncmp(str, filename, ft_strlen(filename)) == 0)
	{
		free(str);
		close(fd);
		return (1);
	}
	return (0);
}

// void apply_heredoc()
// {
// 	int fd;
// 	char *line;

// 	fd = open("tmp/.heredoc_tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
// 	if (fd < 0)
// 		perror("heredoc");
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (heredoc_cheker(line, "<<", fd))
// 			break;
// 		write(fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 		if (line)
// 			free(line);
// 	}
// 	close(fd);
// }

// int run_heredoc(t_mshell *shell, t_tnode *root)
// {
// 	pid_t pid;
// 	int status;

// 	status = 0;
// 	pid = fork();
// 	if (pid == 0)
// 		apply_heredoc();
// 	waitpid(pid, &shell->exit_value, 0);
// 	if (WIFEXITED(shell->exit_value))
// 		shell->exit_value = WEXITSTATUS(shell->exit_value);
// 	return (status);
// }

// int ft_heredoc(t_mshell *shell, t_tnode *root)
// {
// 	(void)shell;
// 	(void)root;
// 	printf("herdoc\n");
// 	return (0);
// }