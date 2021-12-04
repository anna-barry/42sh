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

int exec_pipe(char *argv_left[], char *argv_right[], int count_left,
              int count_right)
{
    int fd[2];
    if (pipe(fd) == -1)
        return 1;
    // char *nb = argv_left[0];
    // char *nb2 = argv_right[0];
    // int pid = fork();
    // if (pid < 0)
    // {
    //     return 1;
    // }
    // if (pid == 0)
    // {
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    // if (execvp(nb, argv_left) == -1)
    //     return 127;
    command_exec(argv_left, count_left);
    // }

    // int pid2 = fork();
    // if (pid2 < 0)
    //     return 3;
    // if (pid2 == 0)
    // {
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);
    return command_exec(argv_right, count_right);
    // if (execvp(nb2, argv_right) == -1)
    //     return 127;
    // }
    close(fd[0]);
    close(fd[1]);
    // waitpid(pid, NULL, 0);
    // int wstatus;
    // waitpid(pid2, &wstatus, 0);
    // if (WEXITSTATUS(wstatus) == 127 || WEXITSTATUS(wstatus) == 0)
    //     return WEXITSTATUS(wstatus);
    // return 1;
}
int main()
{
    char *argv_left[3] = { "echo", "Hallo" };
    char *argv_right[4] = { "tr", "a", "e" };
    exec_pipe(argv_left, argv_right, 2, 3);
    return 0;
}
