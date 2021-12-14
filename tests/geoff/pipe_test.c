#include <err.h>
#include <string.h>

#include "../../src/commands/command_pipe.h"

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

int main(int argc, char const *argv[])
{
    char *argv0_left[2] = { "ls" };
    char *argv0_right[3] = { "cat", "-e" };

    char *argv1_left[3] = { "echo", "Hello" };
    char *argv1_right[3] = { "cat", "-e" };

    char *argv2_left[3] = { "echo", "Hallo" };
    char *argv2_right[4] = { "tr", "a", "e" };

    char *argv3_left[3] = { "echo", "a" };
    char *argv3_right[3] = { "echo", "b" };

    char *argv4_left[2] = { "ls" };
    char *argv4_right[4] = { "tr", "a", "e" };
    if (argc != 2)
        err(1, "Wrong args, try again with  : (./pipe test-*)\n");
    else
    {
        if (strcmp("test0", argv[1]) == 0) // simple echo
            return pipe_exec(argv0_left, argv0_right, 1, 2);
        else if (strcmp("test1", argv[1]) == 0)
            return pipe_exec(argv1_left, argv1_right, 2, 2);
        else if (strcmp("test2", argv[1]) == 0)
            return pipe_exec(argv2_left, argv2_right, 2, 3);
        else if (strcmp("test3", argv[1]) == 0)
            return pipe_exec(argv3_left, argv3_right, 2, 2);
        else if (strcmp("test4", argv[1]) == 0)
            return pipe_exec(argv4_left, argv4_right, 1, 3);
        else
            err(1, "Not implemented test");
    }
    return 0;
}
