#include "../../includes/mshell.h"

void run_pipe(t_tnode *root, t_mshell *shell)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid_left = fork();
    if (pid_left == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid_left == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        ft_execute_tree(root->t_left, shell);
        exit(EXIT_SUCCESS);
    }

    pid_t pid_right = fork();
    if (pid_right == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid_right == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        ft_execute_tree(root->t_right, shell);
        exit(EXIT_SUCCESS);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid_left, NULL, 0);
    waitpid(pid_right, NULL, 0);
}