#include "command.h"

#include <err.h>

#include "echo.h"
//#include <errno.h>
//#include <fcntl.h>
//#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/stat.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int command_exec(char *argv[],
                 int count) // the command should be as : char *argv[4] = {
                            // "echo", "geoffroy", "geoffroy", NULL };
                            // the NULL argument is nessessary.
{
    if (strcmp("echo", argv[0]) == 0)
        return echo(argv, count);
    else
    {
        // argv[count] = NULL;
        int wstatus;
        int res_exec;
        int pid = fork();
        char *command = argv[0];
        if (pid == 0)
        {
            res_exec = execvp(command, argv);
            if (res_exec == -1)
                return 127;
        }
        else
        {
            int res_exec = waitpid(pid, &wstatus, 0);
            if (res_exec == -1)
                err(2, "problem in child");
            if (WIFEXITED(wstatus))
            {
                if (WEXITSTATUS(wstatus) == 127)
                {
                    fprintf(stderr, "Error");
                    return 1;
                }
            }
        }
        return WEXITSTATUS(wstatus); // return the return value of the command
    }
}
// int main(int argc, char *argv[])
// {
//     if (argc == 0)
//     {
//         printf("I'm not sure about that one\n");
//     }
//     return command_exec(argv);
//     return 0;
// }

// int main()
// {
//     char *argv[4] = { "echo", "geoffroy", "geoffroy", NULL };
//     return command_exec(argv);
//     return 0;
// }
/*
int main(int argc, char const *argv[])
{
    char *argv0[2] = { "ls", NULL };
    char *argv1[2] = { "ls", "tests" };
    char *argv2[2] = { "cat", "tests/run_tests_echo" };
    char *argv3[3] = { "echo", "geoffroy", NULL };
    char *argv4[2] = { "ls", "notExistantFile" }; // err will occur and return 1
    char *argv5[2] = { "ld" }; // err will occur and return 1
    if (argc != 2)
        err(1, "Wrong args, try again with  : (./command_exec test-*)\n");
    else
    {
        if (strcmp("test0", argv[1]) == 0) // simple command_exec
            return command_exec(argv0, 1);
        else if (strcmp("test1", argv[1]) == 0)
            return command_exec(argv1, 2);
        else if (strcmp("test2", argv[1]) == 0)
            return command_exec(argv2, 2);
        else if (strcmp("test3", argv[1]) == 0)
            return command_exec(argv3, 2);
        else if (strcmp("test4", argv[1]) == 0)
            return command_exec(argv4, 2);
        else if (strcmp("test5", argv[1]) == 0)
            return command_exec(argv5, 2);
        else
            err(1, "Not implemented test");
    }
    return 0;
}
*/
