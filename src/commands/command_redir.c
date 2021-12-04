#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"

int command_redir_r(char *command[], int count, char *file)
{
    int old_fd = dup(STDOUT_FILENO);
    int fd = open(file, O_CREAT | O_WRONLY, 0644);
    if (dup2(fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (command_exec(command, count) != 0)
        return 127;
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    return 0;
}

int command_redir_l(char *command[], int count, char *file)
{
    int old_fd = dup(STDOUT_FILENO);
    int fd = open(file, O_CREAT | O_WRONLY, 0644);
    if (dup2(fd, STDIN_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (command_exec(command, count) != 0)
        return 127;
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    return 0;
}

int command_redir_rr(char *command[], int count, char *file)
{
    int old_fd = dup(STDOUT_FILENO);
    int fd = open(file, O_CREAT | O_APPEND, 0644);
    if (dup2(fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (command_exec(command, count) != 0)
        return 127;
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    return 0;
}

int command_redir_lr(char *command[], int count, char *file)
{
    int old_fd = dup(STDOUT_FILENO);
    int fd = open(file, O_CREAT | O_RDWR, 0644);
    if (dup2(fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (command_exec(command, count) != 0)
        return 127;
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    return 0;
}

int command_redir_r_and(/*char *command, int count, char *file*/ int out,
                        int in) // basic case 0 for out if NULL
{
    // int n;
    // if (command == NULL)
    //     n = 0;
    // else
    //     n = command;
    // int old_fd = dup(STDOUT_FILENO);
    // int fd = open(file, O_CREAT | O_RDWR, 0644);
    // if (dup2(fd, n) == -1)
    //     fprintf(stderr, "Error with dup2");
    // if (command_exec(command, count) != 0)
    //     return 127;
    // if (dup2(old_fd, STDOUT_FILENO) == -1)
    //     fprintf(stderr, "Error with dup2");
    // fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    // return 0;
    int old_fd = dup(STDOUT_FILENO);
    if (dup2(in, out) == -1)
        fprintf(stderr, "Error with dup2");
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    return 0;
}

int command_redir_r_and(/*char *command, int count, char *file*/ int out,
                        int in) // basic case 1 for out if NULL
{
    // int n;
    // if (command == NULL)
    //     n = 0;
    // else
    //     n = command;
    // int old_fd = dup(STDOUT_FILENO);
    // int fd = open(file, O_CREAT | O_RDWR, 0644);
    // if (dup2(fd, n) == -1)
    //     fprintf(stderr, "Error with dup2");
    // if (command_exec(command, count) != 0)
    //     return 127;
    // if (dup2(old_fd, STDOUT_FILENO) == -1)
    //     fprintf(stderr, "Error with dup2");
    // fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    // return 0;
    int old_fd = dup(STDOUT_FILENO);
    if (dup2(in, out) == -1)
        fprintf(stderr, "Error with dup2");
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    return 0;
}

int main()
{
    char *argv[2] = { "echo", "geoffroy" };
    char *file = "tester.txt";
    printf("recieve\n");
    // fprintf(fd, "new file descriptor\n");
    command_redir(argv, 2, file);
    // fprintf(fd, "old file descriptor\n");
    printf("lounch\n");
    return 0;
}
