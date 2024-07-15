#include "../../includes/mshell.h"

char **get_path(char *path)
{
	char **path_arr;
	char *tmp;
	int i;

	i = 0;
	if (!path)
		return (NULL);
	tmp = ft_strdup(path);
	path_arr = ft_split(tmp, ':');
	free(tmp);
	return (path_arr);
}
char **cmd_args_getter(t_cmd *cmd)
{
	char **cmd_args;
	t_cmd *tmp;
	int i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	cmd_args = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	tmp = cmd;
	while (tmp)
	{
		cmd_args[i] = ft_strdup(tmp->arg);
		i++;
		tmp = tmp->next;
	}
	cmd_args[i] = NULL;
	return (cmd_args);
}



void run_curr(char **cmd_args, char **paths, char **envp)
{
	if (access(cmd_args[0], F_OK | X_OK) == 0 || (cmd_args[0][0] == '.' || cmd_args[0][0] == '/'))
	{
		if (execve(cmd_args[0], cmd_args, envp) == -1)
		{
			ft_printf("minishell: %s: %s\n", cmd_args[0], strerror(errno));
			free_func(cmd_args);
			if (paths)
				free_func(paths);
			if (envp)
				free_func(envp);
			exit(1);
		}
	}
}

char **get_envp(t_env *env)
{
	char **envp;
	char *tmp_str;

	t_env *tmp;
	int i;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!envp)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->key)
		{
			tmp_str = ft_strjoin(tmp->key, "=");
			if (tmp->value)
				envp[i] = ft_strjoin(tmp_str, tmp->value);
			else
				envp[i] = ft_strdup(tmp_str);
			free(tmp_str);
		}
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	*check_command(char *cmd, char **paths)
{
	char	*path_part;
	char	*command;

	command = NULL;
	path_part = NULL;
	if (!paths || !cmd)
		return (NULL);
	while (*paths)
	{
		path_part = ft_strjoin(*paths, "/");
		command = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(command, F_OK | X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

void cmd_runner(t_cmd *cmd, t_mshell *shell)
{
	char **cmd_args;
	char **path;
	char **envp;
	char *cmd_path;
	pid_t pid;

	path = NULL;
	cmd_args = NULL;
	envp = NULL;
	cmd_path = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		cmd_args = cmd_args_getter(cmd);
		if (find_env(shell->env, "PATH"))
			path = get_path(find_env(shell->env, "PATH")->value);
		envp = get_envp(shell->env);
		run_curr(cmd_args, path, envp);
		cmd_path = check_command(cmd_args[0], path);
		if (cmd_path == NULL)
		{
			print_stderr("command not found");
			free_func(cmd_args);
			if (path)
				free_func(path);
			if (envp)
				free_func(envp);
			exit(127);
		}
		if (execve(cmd_path, cmd_args, envp) == -1)
			(perror(""), exit(1));
	}
	else
	{
		waitpid(pid, &shell->exit_value, 0);
		if (WIFEXITED(shell->exit_value))
			shell->exit_value = WEXITSTATUS(shell->exit_value);
	}
}

// void double_dup(int fd1, int fd2)
// {
// 	if (dup2(fd1, STDIN_FILENO) == -1)
// 		perror("dup2");
// 	if (dup2(fd2, STDOUT_FILENO) == -1)
// 		perror("dup2");
// }

void ft_execute_cmd(t_tnode *root, t_mshell *shell)
{
	t_cmd *cmd;

	cmd = root->cmd;
	if (builtins_finder(cmd, shell, builtins_checker(cmd)) == -1)
		cmd_runner(cmd, shell);
}

// void ft_execute_pipe(t_tnode *root, t_mshell *shell, int f)
// {
// 	if (f == 1)
// 		printf("left\n");
// 	else if (f == 2)
// 		printf("right\n");

// 	if (root->cmd)
// 	{
// 		printf("parent type (%d)\n", root->t_parent->node_type);
// 		printf("cmd type (%d) -> %s\n", root->node_type, root->cmd->arg);
// 		if (root->redirection->out_file) {
// 			printf("out file -> %s\n", root->redirection->out_file->filename);
// 			printf("mode -> %d\n", root->redirection->out_file->mode);
// 		}
// 	}
// 	else
// 	{
// 		ft_execute_pipe(root->t_left, shell, 1);
// 		ft_execute_pipe(root->t_right, shell, 2);
// 	}
// }

void reset_in_out(int stdin, int stdout)
{
	if (dup2(stdin, STDIN_FILENO) == -1)
		perror("dup2");
	if (dup2(stdout, STDOUT_FILENO) == -1)
		perror("dup2");
	close(stdin);
	close(stdout);
}

// TODO: this functions checks the number of cmds in the tree
int cmd_counter(t_tnode *root)
{
	int count;

	count = 0;
	if (root)
	{
		if (root->node_type == TOKEN_PIPE)
		{
			count += cmd_counter(root->t_left);
			count += cmd_counter(root->t_right);
		}
		if (root->node_type == TOKEN_WORD)
			count++;
	}
	return (count);
}

// void ft_execute_tree(t_tnode *root, t_mshell *shell)
// {
// 	int stdout;
// 	int stdin;

// 	stdout = dup(STDOUT_FILENO);
// 	stdin = dup(STDIN_FILENO);
// 	if (root == NULL)
// 		return;
// 	//TODO: run herdox first no mater where is it
// 	//TODO: see if there is pipe(fork and run the pipes)
// 	//TODO: handel the redirections

// 	// printf("cmd counter %d\n", cmd_counter(root));
// 	// if (cmd_counter(root) == 1) {
// 	// 	cmd_runner(root->cmd, shell);
// 	// 	return;
// 	// }
	
// 	if (root)
// 	{
// 		// and -> 3
// 		// or -> 2
// 		// pipe -> 1
// 		// l_parenthise -> 4
// 		// r_parenthise -> 5
// 		// append -> 6
// 		// herdoc -> 7
// 		// in_redirection -> 8
// 		// out_redirection -> 9
// 		if (root->node_type == TOKEN_PIPE) {
// 			ft_execute_tree(root->t_left, shell);
// 			ft_execute_tree(root->t_right, shell);
// 		}
// 		// if (shell->n_herdoc > 0)
// 		// 	ft_heredoc(root, shell);
// 		if (root->node_type == TOKEN_WORD) {
// 			ft_execute_cmd(root, shell);
// 			// printf("display command %s\n", root->cmd->arg);
// 		}
// 			// ft_execute_cmd(root, shell);
// 	}
// 	reset_in_out(stdin, stdout);
// }

// this functions create n file to use for heredoc


void handle_input_redirection(t_infile *in_file, t_mshell *shell)
{
	int here_doc_num;
	int fd;

	here_doc_num = 0;
    while (in_file)
	{
        if (in_file->mode == 7)
		{
			fd = open(ft_strjoin("/tmp/heredoc_", ft_itoa(here_doc_num)), O_RDONLY);
			here_doc_num++;
		}
		else if (in_file->mode == 8) 
		{
            fd = open(in_file->filename, O_RDONLY);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
        in_file = in_file->next;
    }
}

void handle_output_redirection(t_outfile *out_file, t_mshell *shell)
{
	int fd;

    while (out_file) {
		if (out_file->mode == 9)
			fd = open(out_file->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (out_file->mode == 6)
			fd = open(out_file->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
        out_file = out_file->next;
    }
}

void handle_word(t_tnode *root, t_mshell *shell) {
    if (root->redirection->in_file)
        handle_input_redirection(root->redirection->in_file, shell);
    if (root->redirection->out_file)
        handle_output_redirection(root->redirection->out_file, shell);
    ft_execute_cmd(root, shell);
}

void ft_execute_tree(t_tnode *root, t_mshell *shell)
{
    int (stdout_fd), (stdin_fd);
    stdout_fd = dup(STDOUT_FILENO);
    stdin_fd = dup(STDIN_FILENO);
    if (root == NULL) {
		reset_in_out(stdin_fd, stdout_fd);
        return;
	}
    if (root->node_type == TOKEN_PIPE) {
		run_pipe(root, shell);
    }
	else if (root->node_type == TOKEN_WORD) {
		handle_word(root, shell);
	}
	else if (root->node_type == 3) // 3 means there is an and 
	{
		ft_execute_tree(root->t_left, shell);
		if (shell->exit_value == 0)
			ft_execute_tree(root->t_right, shell);
	}
	else if (root->node_type == 2) // 2 means there is an or
	{
		ft_execute_tree(root->t_left, shell);
		if (shell->exit_value != 0)
			ft_execute_tree(root->t_right, shell);
	}
    reset_in_out(stdin_fd, stdout_fd);
}