#include "command.h"

#include <err.h>

#include "../functionnal/functionnal.h"
#include "cd.h"
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

int command_break(struct environnement *env)
{
    env->flag_loop_break -= 1;
    return 0;
}

int command_continue(struct environnement *env)
{
    env->flag_loop_continue = 1;
    return 0;
}

int simple_command_exec(char *argv[], int count)
{
    if (argv[count] != NULL)
    {
        argv[count] = NULL;
    }
    int wstatus = 0;
    int res_exec = 0;
    int pid = fork();
    char *command = argv[0];
    if (pid == 0)
    {
        res_exec = execvp(command, argv);
        if (res_exec == -1)
            return 127;
    }
    if (pid > 0)
    {
        int res_exec = waitpid(pid, &wstatus, 0);
        if (res_exec == -1)
            err(2, "problem in child");
        if (WIFEXITED(wstatus))
            if (WEXITSTATUS(wstatus) == 127)
            {
                fprintf(stderr, "Error");
                return 127;
            }
    }
    if (pid < 0)
    {
        printf("null great \n");
        return -1;
    }
    return WEXITSTATUS(wstatus); // return the return value of the command
}

int command_exec(char *argv[], int count, struct environnement *env)
{
    if (strcmp("echo", argv[0]) == 0)
        return echo(argv, count);

    else if (strcmp("break", argv[0]) == 0)
        return command_break(env);

    else if (strcmp("continue", argv[0]) == 0)
        return command_continue(env);

    else if (strcmp("exit", argv[0]) == 0)
    {
        int start = 0;
        int i = 0;
        while (i < count && argv[1][i] != '\0')
        {
            start = start * 10 + ((int)argv[1][i] - 48);
            i++;
        }
        env->exit_status = start;
        return env->exit_status;
    }
    else if (strcmp("cd", argv[0]) == 0)
    {
        if (count == 2)
            return cd(argv[1]);
        else
            return 1;
    }
    else if (is_dot(argv[0]))
        return my_dot(argv, count, env);

    else
    {
        int res = simple_command_exec(argv, count);
        if (res == 127)
            return 127;
        return res;
    }
}

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
