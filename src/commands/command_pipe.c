#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "echo.h"
#include "parser.h"

// int exec_pipe(char *argv_left[], char *argv_right[], int count_left,
//               int count_right)
// {
//     int fd[2];
//     if (pipe(fd) == -1)
//         return 1;
//     int old_fd = dup(STDOUT_FILENO);
//     if (dup2(fd[1], STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2 for fd1");
//     // close(fd[0]);
//     close(fd[1]);
//     command_exec(argv_left, count_left);
//     if (dup2(fd[0], STDIN_FILENO) == -1)
//         fprintf(stderr, "Error with dup2 for fd0");
//     close(fd[0]);
//     close(fd[1]);
//     command_exec(argv_right, count_right);
//     // close(fd[0]);
//     // close(fd[1]);
//     if (dup2(old_fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     fcntl(old_fd, F_SETFD, FD_CLOEXEC);
//     return 0;
// }
// // flush(stdout) for the builtin functions.
//
// int command_pipe(char *argv_left[], char *argv_right[], int count_left,
//                  int count_right)
// {
//     int old_fd = dup(STDOUT_FILENO);
//     int fd = 0;
//     if (dup2(fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     if (command_exec(argv_left, count_left) != 0)
//         return 127;
//     if (dup2(fd, STDIN_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     close(fd);
//     if (command_exec(argv_right, count_right) != 0)
//         return 127;
//     if (dup2(old_fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     fcntl(old_fd, F_SETFD, FD_CLOEXEC);
//     return 0;
// }
//
// int command_exec_pipe(char *argv_left[], char *argv_right[], int count_left,
//                       int count_right)
// {
//     int old_fd = dup(STDOUT_FILENO);
//     int fd[2];
//     if (pipe(fd) == -1)
//         return 1;
//     if (strcmp(argv_left[0], "echo") == 0)
//     {
//         dup2(fd[1], STDOUT_FILENO);
//         close(fd[1]);
//         close(fd[0]);
//         echo(argv_left, count_left);
//     }
//     else
//     {
//         int pid = fork();
//         if (pid < 0)
//         {
//             return 1;
//         }
//         if (pid == 0)
//         {
//             dup2(fd[1], STDOUT_FILENO);
//             close(fd[0]);
//             close(fd[1]);
//             execvp(argv_left[0], argv_left);
//         }
//         int wstatus1;
//         waitpid(pid, &wstatus1, 0);
//         if (WEXITSTATUS(wstatus1) == 127 || WEXITSTATUS(wstatus1) == 0)
//             return WEXITSTATUS(wstatus1);
//     }
//     if (strcmp(argv_right[0], "echo") == 0)
//     {
//         dup2(fd[0], STDIN_FILENO);
//         close(fd[0]);
//         close(fd[1]);
//         echo(argv_right, count_right);
//     }
//     else
//     {
//         int pid2 = fork();
//         if (pid2 < 0)
//             return 3;
//         if (pid2 == 0)
//         {
//             dup2(fd[0], STDIN_FILENO);
//             close(fd[0]);
//             close(fd[1]);
//             execvp(argv_right[0], argv_right);
//         }
//         close(fd[0]);
//         close(fd[1]);
//         int wstatus;
//         waitpid(pid2, &wstatus, 0);
//         if (WEXITSTATUS(wstatus) == 127 || WEXITSTATUS(wstatus) == 0)
//             return WEXITSTATUS(wstatus);
//
//         return 1;
//     }
//     if (dup2(old_fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     fcntl(old_fd, F_SETFD, FD_CLOEXEC);
//     return 0;
// }

int pipe_ast(struct ast *ast, struct environnement *env)
{
    struct ast_and *pipe = ast->data.ast_pipe;
    int fd[2];
    // argv_left[count_left] = NULL;
    // argv_right[count_right] = NULL;
    if (pipe(fd) == -1)
        return 1;
    int pid = fork();
    if (pid < 0)
    {
        return 1;
    }
    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        exec_ast(pipe->left, env);
    }
    int pid2 = fork();
    if (pid2 < 0)
        return 3;
    if (pid2 == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        exec_ast(pipe->right, env);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid, NULL, 0);
    int wstatus;
    waitpid(pid2, &wstatus, 0);
    if (WEXITSTATUS(wstatus) == 127 || WEXITSTATUS(wstatus) == 0)
        return WEXITSTATUS(wstatus);
    return 1;
}

int pipe_exec(char *argv_left[], char *argv_right[], int count_left,
              int count_right)
{
    int fd[2];
    // argv_left[count_left] = NULL;
    // argv_right[count_right] = NULL;
    if (pipe(fd) == -1)
        return 1;
    int pid = fork();
    if (pid < 0)
    {
        return 1;
    }
    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        if (strcmp(argv_right[0], "echo") == 0)
            echo(argv_left, count_left);
        else
            execvp(argv_left[0], argv_left);
    }
    int pid2 = fork();
    if (pid2 < 0)
        return 3;
    if (pid2 == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        if (strcmp(argv_right[0], "echo") == 0)
            echo(argv_right, count_right);
        else
            execvp(argv_right[0], argv_right);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid, NULL, 0);
    int wstatus;
    waitpid(pid2, &wstatus, 0);
    if (WEXITSTATUS(wstatus) == 127 || WEXITSTATUS(wstatus) == 0)
        return WEXITSTATUS(wstatus);
    return 1;
}

// int main()
// {
//     char *argv_left[2] = { "ls" };
//     char *argv_right[3] = { "cat", "-e" };
//     // command_exec_pipe(argv_left, argv_right, 2, 3);
//     pipe_exec(argv_left, argv_right, 1, 2);
//     // exec_pipe(argv_left, argv_right, 2, 3);
//     // command_pipe(argv_left, argv_right, 2, 3);
//     return 0;
// }
// // cat command.c | cat -e
// // cat -e command.c
