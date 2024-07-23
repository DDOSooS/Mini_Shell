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
	// print agrs 
	for (int j = 0; cmd_args[j] != NULL; j++)
		printf("cmd_args[%d]: %s\n", j, cmd_args[j]);
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

int match_wildcard(const char *pattern, const char *str) {
    if (*pattern == '\0' && *str == '\0')
        return 1;
    if (*pattern == '*' && *(pattern + 1) != '\0' && *str == '\0')
        return 0;
    if (*pattern == '?' || *pattern == *str)
        return match_wildcard(pattern + 1, str + 1);
    if (*pattern == '*')
        return match_wildcard(pattern + 1, str) || match_wildcard(pattern, str + 1);
    return 0;
}

// Function to expand wildcard and store matches in a list
char **expand_wildcard(const char *pattern, const char *cmd) {
    struct dirent *entry;
    DIR *dp;
    char **matches = malloc(MAX_MATCHES * sizeof(char *));
	matches[0] = ft_strdup(cmd);
    int match_count = 1;

    printf("print given pattern %s\n", pattern);
    dp = opendir(".");
    if (dp == NULL) {
        perror("opendir");
        return NULL;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (match_wildcard(pattern, entry->d_name)) {
			if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0)
				continue;
            matches[match_count] = ft_strdup(entry->d_name);
            match_count++;
            if (match_count >= MAX_MATCHES) {
                break;
            }
        }
    }

    closedir(dp);
    matches[match_count] = NULL;
	// print the matches
	for (int i = 0; matches[i] != NULL; i++) {
		printf("matches[%d]: %s\n", i, matches[i]);
	}
    return matches;
}

//TODO: check the command and then get the expended of char ** of the *

void cmd_runner(t_cmd *cmd, t_mshell *shell) {
    char **cmd_args;
    char **path;
    char **envp;
    char *cmd_path;
    pid_t pid;
    int status;
    char **expanded_args = NULL;
    int expanded_count = 0;
    int max_args = 1000; // Adjust this value as needed
    int had_wildcard = 0;
    int wildcard_matched = 0;

    path = NULL;
    cmd_args = NULL;
    envp = NULL;
    cmd_path = NULL;

    handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        handle_signals(active_sigint, active_sigquit, SIG_IGN, SIG_IGN);

        expanded_args = malloc(sizeof(char *) * max_args);
        if (!expanded_args) {
            perror("malloc");
            exit(1);
        }

        expanded_args[expanded_count++] = ft_strdup(cmd->arg);

        t_cmd *current = cmd->next;
        while (current) {
            if (strchr(current->arg, '*') || strchr(current->arg, '?')) {
                had_wildcard = 1;
                char **wildcard_expanded = expand_wildcard(current->arg, cmd->arg);
                if (wildcard_expanded && wildcard_expanded[1] != NULL) { // Check if any matches were found
                    wildcard_matched = 1;
                    for (int i = 1; wildcard_expanded[i] != NULL; i++) {
                        if (expanded_count < max_args - 1) {
                            expanded_args[expanded_count++] = ft_strdup(wildcard_expanded[i]);
                        }
                    }
                    for (int i = 0; wildcard_expanded[i] != NULL; i++) {
                        free(wildcard_expanded[i]);
                    }
                    free(wildcard_expanded);
                } else {
                    // No matches found, add original argument
                    if (expanded_count < max_args - 1) {
                        expanded_args[expanded_count++] = ft_strdup(current->arg);
                    }
                }
            } else {
                if (expanded_count < max_args - 1) {
                    expanded_args[expanded_count++] = ft_strdup(current->arg);
                }
            }
            current = current->next;
        }
        expanded_args[expanded_count] = NULL;

        cmd_args = expanded_args;

        if (find_env(shell->env, "PATH"))
            path = get_path(find_env(shell->env, "PATH")->value);
        envp = get_envp(shell->env);
        
        if (had_wildcard && !wildcard_matched) {
            // If wildcard was used but didn't match anything, print error and exit
            for (int i = 1; cmd_args[i] != NULL; i++) {
                if (strchr(cmd_args[i], '*') || strchr(cmd_args[i], '?')) {
                    fprintf(stderr, "%s: cannot access '%s': No such file or directory\n", cmd_args[0], cmd_args[i]);
                }
            }
            exit(1);
        }

        run_curr(cmd_args, path, envp);
        
        cmd_path = check_command(cmd_args[0], path);
        if (cmd_path == NULL) {
            ft_putstr_fd("minishell (101): ", 2);
            ft_putstr_fd(cmd_args[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            free_func(cmd_args);
            if (path)
                free_func(path);
            if (envp)
                free_func(envp);
            exit(127);
        }
        if (execve(cmd_path, cmd_args, envp) == -1)
            (perror("here Error:"), exit(1));
    } else {
        waitpid(pid, &status, 0);
        if (status == 2 || status == 131) {
            if (status == 2)
                shell->exit_value = 130;
            else
                printf("Quit (core dumped)");
            printf("\n");
        } else
            shell->exit_value = get_status(status);
    }
    handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);

    if (expanded_args) {
        for (int i = 0; expanded_args[i] != NULL; i++) {
            free(expanded_args[i]);
        }
        free(expanded_args);
    }
}


// int match_wildcard(const char *pattern, const char *str) {
//     if (*pattern == '\0' && *str == '\0')
//         return 1;
//     if (*pattern == '*' && *(pattern + 1) != '\0' && *str == '\0')
//         return 0;
//     if (*pattern == '?' || *pattern == *str)
//         return match_wildcard(pattern + 1, str + 1);
//     if (*pattern == '*')
//         return match_wildcard(pattern + 1, str) || match_wildcard(pattern, str + 1);
//     return 0;
// }

// // Function to expand wildcard and store matches in a list
// char **expand_wildcard(const char *pattern, const char *cmd) {
//     struct dirent *entry;
//     DIR *dp;
//     char **matches = malloc(MAX_MATCHES * sizeof(char *));
// 	// matches[0] = ft_strdup(cmd);
//     int match_count = 0;

//     printf("print given pattern %s\n", pattern);
//     dp = opendir(".");
//     if (dp == NULL) {
//         perror("opendir");
//         return NULL;
//     }
//     while ((entry = readdir(dp)) != NULL) {
//         if (match_wildcard(pattern, entry->d_name)) {
// 			if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0)
// 				continue;
//             matches[match_count] = ft_strdup(entry->d_name);
//             match_count++;
//             if (match_count >= MAX_MATCHES) {
//                 break;
//             }
//         }
//     }

//     closedir(dp);
//     matches[match_count] = NULL;
// 	// print the matches
// 	for (int i = 0; matches[i] != NULL; i++) {
// 		printf("matches[%d]: %s\n", i, matches[i]);
// 	}
//     return matches;
// }

// void cmd_runner(t_cmd *cmd, t_mshell *shell) {
//     char **cmd_args;
//     char **path;
//     char **envp;
//     char *cmd_path;
//     pid_t pid;
//     int status;
//     char **expanded_args = NULL;

//     path = NULL;
//     cmd_args = NULL;
//     envp = NULL;
//     cmd_path = NULL;

//     // exec_signal(&shell->sig);
//     handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);
//     pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         exit(1);
//     }
//     if (pid == 0) {
//         handle_signals(active_sigint, active_sigquit, SIG_IGN, SIG_IGN);

//         if (cmd->next && cmd->next->check_wildcard) {
//             expanded_args = expand_wildcard(cmd->next->arg, cmd->arg);
//             if (!expanded_args) {
//                 fprintf(stderr, "Wildcard expansion failed\n");
//                 exit(1);
//             }
//             cmd_args = expanded_args;
//         } else {
//             cmd_args = cmd_args_getter(cmd);
//         }

//         if (find_env(shell->env, "PATH"))
//             path = get_path(find_env(shell->env, "PATH")->value);
//         envp = get_envp(shell->env);
// 		printf("before \n");
//         run_curr(cmd_args, path, envp);
// 		printf("after \n");
//         cmd_path = check_command(cmd_args[0], path);
//         if (cmd_path == NULL) {
//             ft_putstr_fd("minishell (101): ", 2);
//             ft_putstr_fd(cmd_args[0], 2);
//             ft_putstr_fd(": command not found\n", 2);
//             free_func(cmd_args);
//             if (path)
//                 free_func(path);
//             if (envp)
//                 free_func(envp);
//             exit(127);
//         }
//         if (execve(cmd_path, cmd_args, envp) == -1)
//             (perror("here Error:"), exit(1));
//     } else {
//         waitpid(pid, &status, 0);
//         if (status == 2 || status == 131) {
//             if (status == 2)
//                 shell->exit_value = 130;
//             else
//                 printf("Quit (core dumped)");
//             printf("\n");
//         } else
//             shell->exit_value = get_status(status);
//     }
//     handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);

//     // Free expanded arguments
//     if (expanded_args) {
//         for (int i = 0; expanded_args[i] != NULL; i++) {
//             free(expanded_args[i]);
//         }
//         free(expanded_args);
//     }
// }


// void cmd_runner(t_cmd *cmd, t_mshell *shell) {
//     char **cmd_args;
//     char **path;
//     char **envp;
//     char *cmd_path;
//     pid_t pid;
//     int status;
//     char **expanded_args = NULL;
//     int expanded_count = 0;
//     int max_args = 1000; // Adjust this value as needed
//     int had_wildcard = 0;
//     int wildcard_matched = 0;

//     path = NULL;
//     cmd_args = NULL;
//     envp = NULL;
//     cmd_path = NULL;

//     handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);
//     pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         exit(1);
//     }
//     if (pid == 0) {
//         handle_signals(active_sigint, active_sigquit, SIG_IGN, SIG_IGN);

//         expanded_args = malloc(sizeof(char *) * max_args);
//         if (!expanded_args) {
//             perror("malloc");
//             exit(1);
//         }

//         expanded_args[expanded_count++] = ft_strdup(cmd->arg);

//         t_cmd *current = cmd->next;
//         while (current) {
//             if (strchr(current->arg, '*') || strchr(current->arg, '?')) {
//                 had_wildcard = 1;
//                 char **wildcard_expanded = expand_wildcard(current->arg, cmd->arg);
//                 if (wildcard_expanded && wildcard_expanded[1] != NULL) { // Check if any matches were found
//                     wildcard_matched = 1;
//                     for (int i = 1; wildcard_expanded[i] != NULL; i++) {
//                         if (expanded_count < max_args - 1) {
//                             expanded_args[expanded_count++] = ft_strdup(wildcard_expanded[i]);
//                         }
//                     }
//                     for (int i = 0; wildcard_expanded[i] != NULL; i++) {
//                         free(wildcard_expanded[i]);
//                     }
//                     free(wildcard_expanded);
//                 } else {
//                     // No matches found, add original argument
//                     if (expanded_count < max_args - 1) {
//                         expanded_args[expanded_count++] = ft_strdup(current->arg);
//                     }
//                 }
//             } else {
//                 if (expanded_count < max_args - 1) {
//                     expanded_args[expanded_count++] = ft_strdup(current->arg);
//                 }
//             }
//             current = current->next;
//         }
//         expanded_args[expanded_count] = NULL;

//         cmd_args = expanded_args;

//         if (find_env(shell->env, "PATH"))
//             path = get_path(find_env(shell->env, "PATH")->value);
//         envp = get_envp(shell->env);
        
//         if (had_wildcard && !wildcard_matched) {
//             // If wildcard was used but didn't match anything, print error and exit
//             for (int i = 1; cmd_args[i] != NULL; i++) {
//                 if (strchr(cmd_args[i], '*') || strchr(cmd_args[i], '?')) {
// 					printf("%s: %s: No match found\n", cmd_args[0], cmd_args[i]);
//                     fprintf(stderr, "%s: cannot access '%s': No such file or directory\n", cmd_args[0], cmd_args[i]);
//                 }
//             }
//             exit(1);
//         }

//         run_curr(cmd_args, path, envp);
        
//         cmd_path = check_command(cmd_args[0], path);
//         if (cmd_path == NULL) {
//             ft_putstr_fd("minishell (101): ", 2);
//             ft_putstr_fd(cmd_args[0], 2);
//             ft_putstr_fd(": command not found\n", 2);
//             free_func(cmd_args);
//             if (path)
//                 free_func(path);
//             if (envp)
//                 free_func(envp);
//             exit(127);
//         }
//         if (execve(cmd_path, cmd_args, envp) == -1)
//             (perror("here Error:"), exit(1));
//     } else {
//         waitpid(pid, &status, 0);
//         if (status == 2 || status == 131) {
//             if (status == 2)
//                 shell->exit_value = 130;
//             else
//                 printf("Quit (core dumped)");
//             printf("\n");
//         } else
//             shell->exit_value = get_status(status);
//     }
//     handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);

//     if (expanded_args) {
//         for (int i = 0; expanded_args[i] != NULL; i++) {
//             free(expanded_args[i]);
//         }
//         free(expanded_args);
//     }
// }


// void cmd_runner(t_cmd *cmd, t_mshell *shell)
// {
// 	char **cmd_args;
// 	char **path;
// 	char **envp;
// 	char *cmd_path;
// 	pid_t pid;
// 	int status;

// 	path = NULL;
// 	cmd_args = NULL;
// 	envp = NULL;
// 	cmd_path = NULL;

// 	// exec_signal(&shell->sig);
// 	handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);	
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		exit(1);
// 	}
// 	if (pid == 0)
// 	{
// 		// child_sig(&shell->sig);
// 		handle_signals(active_sigint, active_sigquit, SIG_IGN, SIG_IGN);
// 		cmd_args = cmd_args_getter(cmd);
// 		if (find_env(shell->env, "PATH"))
// 			path = get_path(find_env(shell->env, "PATH")->value);
// 		envp = get_envp(shell->env);
// 		run_curr(cmd_args, path, envp);
// 		cmd_path = check_command(cmd_args[0], path);
// 		if (cmd_path == NULL)
// 		{
// 			ft_putstr_fd("minishell: ", 2);
// 			ft_putstr_fd(cmd_args[0], 2);
// 			ft_putstr_fd(": command not found\n", 2);
// 			free_func(cmd_args);
// 			if (path)
// 				free_func(path);
// 			if (envp)
// 				free_func(envp);
// 			exit(127);
// 		}
// 		if (execve(cmd_path, cmd_args, envp) == -1)
// 			(perror(""), exit(1));
// 	}
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		if (status == 2 || status == 131)
// 		{
// 			if (status == 2)
// 				shell->exit_value = 130;
// 			else
// 				printf("Quit (core dumped)");
// 			printf("\n");
// 		}
// 		else
// 			shell->exit_value = get_status(status);
// 	}
// 	handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
// }

char *get_last_arg(t_cmd *cmd)
{
	t_cmd *tmp;

	tmp = cmd;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp->arg);
}

void set_under_score(t_env *env, t_cmd *cmd)
{
	t_env *env_under_score;

	env_under_score = find_env(env, "_");
	if (env_under_score)
	{
		if (count_args(cmd) > 0)
		{
			free(env_under_score->value);
			env_under_score->value = ft_strdup(get_last_arg(cmd));
		}
		else
		{
			free(env_under_score->value);
			env_under_score->value = ft_strdup("");
		}
	}
}

void ft_execute_cmd(t_tnode *root, t_mshell *shell)
{
	t_cmd *cmd;
	int status;

	cmd = root->cmd;
	if (!cmd)
		return;
	status = builtins_finder(cmd, shell, builtins_checker(cmd));
	if (status == -1)
		cmd_runner(cmd, shell);
	else
		shell->exit_value = status;
	set_under_score(shell->env, root->cmd);
}

void reset_in_out(int stdin, int stdout)
{
	if (dup2(stdin, STDIN_FILENO) == -1)
		perror("dup2");
	if (dup2(stdout, STDOUT_FILENO) == -1)
		perror("dup2");
	close(stdin);
	close(stdout);
}


int handle_input_redirection(t_infile *in_file, t_mshell *shell)
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
				printf("minishell (in): %s: %s\n", in_file->filename, strerror(errno));
				return (-1);
            }
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
        in_file = in_file->next;
    }
	return (0);
}

int handle_output_redirection(t_outfile *out_file, t_mshell *shell)
{
	int fd;

    while (out_file) {
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
		return (handle_input_redirection(root->redirection->in_file, shell));
	if (root->redirection->out_file)
		return (handle_output_redirection(root->redirection->out_file, shell));
	return (0);
}

void handle_word(t_tnode *root, t_mshell *shell)
{
	if (apply_redirections(root, shell) == -1)
	{
		shell->exit_value = 1;
		printf("this should be applied\n");
		return;
	}
    ft_execute_cmd(root, shell);
}


void ft_execute_parenthises(t_tnode *root, t_mshell *shell)
{
	int pid;

	if (apply_redirections(root, shell) == -1)
	{
		shell->exit_value = 1;
		return;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		ft_execute_tree(root->t_left, shell);
		exit(shell->exit_value);
	}
	else
	{
		waitpid(pid, &shell->exit_value, 0);
		shell->exit_value = get_status(shell->exit_value);
	}
}

void ft_execute_tree(t_tnode *root, t_mshell *shell)
{
    int (stdout_fd), (stdin_fd);
    stdout_fd = dup(STDOUT_FILENO);
    stdin_fd = dup(STDIN_FILENO);
    if (root == NULL) 
	{
		reset_in_out(stdin_fd, stdout_fd);
        return;
	}
    if (root->node_type == TOKEN_PIPE)
		run_pipe(root, shell);
	else if (root->node_type == TOKEN_PARENTHISE)
		ft_execute_parenthises(root, shell);
	else if (root->node_type == TOKEN_WORD)
		handle_word(root, shell);
	else if (root->node_type == TOKEN_AND) // 3 means there is an and 
	{
		ft_execute_tree(root->t_left, shell);
		if (shell->exit_value == 0)
			ft_execute_tree(root->t_right, shell);
	}
	else if (root->node_type == TOKEN_OR) // 2 means there is an or
	{
		ft_execute_tree(root->t_left, shell);
		if (shell->exit_value != 0)
			ft_execute_tree(root->t_right, shell);
	}
    reset_in_out(stdin_fd, stdout_fd);
}


void execute(t_tnode *root, t_mshell *shell)
{
	if (shell->n_herdoc > 0)
	{
		ft_heredoc(root, shell);
		shell->n_herdoc = 0;
	}
	ft_execute_tree(root, shell);
}