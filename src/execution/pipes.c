#include "../../includes/mshell.h"

// void run_pipe(t_tnode *root, t_mshell *shell)
// {
//     int pipefd[2];
//     if (pipe(pipefd) == -1) {
//         perror("pipe");
//         exit(EXIT_FAILURE);
//     }

//     pid_t pid_left = fork();
//     if (pid_left == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     } else if (pid_left == 0) {
//         close(pipefd[0]);
//         dup2(pipefd[1], STDOUT_FILENO);
//         close(pipefd[1]);
//         ft_execute_tree(root->t_left, shell);
//         exit(EXIT_SUCCESS);
//     }

//     pid_t pid_right = fork();
//     if (pid_right == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     } else if (pid_right == 0) {
//         close(pipefd[1]);
//         dup2(pipefd[0], STDIN_FILENO);
//         close(pipefd[0]);
//         ft_execute_tree(root->t_right, shell);
//         exit(EXIT_SUCCESS);
//     }
//     close(pipefd[0]);
//     close(pipefd[1]);
//     waitpid(pid_left, NULL, 0);
//     waitpid(pid_right, NULL, 0);
// }

void create_pipe(int pipefd[2])
{
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

static int fork_process(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}

void setup_left_child(int pipefd[2], t_tnode *root, t_mshell *shell)
{
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    ft_execute_tree(root->t_left, shell);
    // the child process will exit after executing the command with the exit value of the executed command
    exit(shell->exit_value);
}

void setup_right_child(int pipefd[2], t_tnode *root, t_mshell *shell)
{
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    ft_execute_tree(root->t_right, shell);
    exit(shell->exit_value);
}

void run_pipe(t_tnode *root, t_mshell *shell)
{
    int pipefd[2];
    int (pid_left), (pid_right), (status);

    create_pipe(pipefd);
    pid_left = fork_process();
    if (pid_left == 0)
        setup_left_child(pipefd, root, shell);
    pid_right = fork_process();
    if (pid_right == 0)
        setup_right_child(pipefd, root, shell);
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid_left, &status, 0);
    waitpid(pid_right, &status, 0);
    shell->exit_value = get_status(status);
}
