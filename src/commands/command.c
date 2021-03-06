#include "command.h"

#include <err.h>

#include "../exec/concat.h"
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
    // printf("break \n");
    // env->flag_loop_break -= 1;
    env->flag_loop_break = 1;
    return 0;
}

int command_continue(struct environnement *env)
{
    // printf("continue \n");
    env->flag_loop_continue = 1;
    return 0;
}

int simple_command_exec(char *argv[], int count)
{
    if (argv[count - 1] != NULL)
    {
        argv[count - 1] = NULL;
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
                fprintf(stderr, "Error: command doesn't exist\n");
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

int command_exec(struct ast *ast, int count, struct environnement *env)
{
    struct ast_command *asttt = ast->data.ast_command;
    char **argv = asttt->argv;
    enum token_type *type = asttt->type;
    if (argv[count - 1] != NULL)
        count += 1;
    char **argv_cpy = malloc(sizeof(char *) * count);
    argv_cpy[count - 1] = NULL;
    int a = 0;
    for (; a < count - 1; a++)
    {
        if (argv[a] != NULL)
        {
            argv_cpy[a] = malloc(sizeof(char) * (strlen(argv[a]) + 1));
            memcpy(argv_cpy[a], argv[a], (strlen(argv[a]) + 1));
        }
    }
    for (int j = 0; j < count - 1; j++)
    {
        if (argv_cpy[j] == NULL)
            break;
        for (int index = 0; index < (int)strlen(argv_cpy[j]); index++)
        {
            if (index > 0 && argv_cpy[j][index - 1] == '\\'
                && argv_cpy[j][index] == '$')
            {
                int i = index - 1;
                for (; argv_cpy[j][i + 1] != '\0'; i++)
                {
                    argv_cpy[j][i] = argv_cpy[j][i + 1];
                }
                argv_cpy[j][i] = '\0';
            }
            // printf("type of quote = %d when = %s\n", type[j], argv_cpy[j]);
            if (argv_cpy[j][index] == '$' && type[j] != TOKEN_SIMPLE_QUOTE
                && type[j] != TOKEN_FOR_SINGLE_QUOTE)
            {
                char *new = transform_char(argv_cpy[j], env, &index);
                argv_cpy[j] = new;
                break;
            }
            if (argv_cpy[j] == NULL)
                break;
        }
    }
    int res = 0;
    if (argv_cpy[0] == NULL)
    {
        err(2, "command missing\n");
    }
    else if (strcmp("echo", argv_cpy[0]) == 0)
        res = echo(argv_cpy, count);

    else if (strcmp("break", argv_cpy[0]) == 0)
    {
        res = command_break(env);
    }

    else if (strcmp("continue", argv_cpy[0]) == 0)
        res = command_continue(env);

    else if (strcmp("exit", argv_cpy[0]) == 0)
    {
        // printf("exit\n");
        // printf("count = %d\n", count);
        int start = 0;
        int i = 0;
        if (count == 2)
        {
            start = 0;
            env->exit_status = start;
            res = env->exit_status;
        }
        else if (count == 3)
        {
            // printf("ici\n");
            while (argv_cpy[1][i] != '\0' && argv_cpy[1][i] >= '0'
                   && argv_cpy[1][i] <= '9')
            {
                i++;
            }
            if (i == (int)strlen(argv_cpy[1]))
            {
                start = atoi(argv_cpy[1]);
                env->exit_status = start;
                res = env->exit_status;
            }
            else
                res = 2;
        }
        else
            res = 2;
    }
    else if (strcmp("cd", argv_cpy[0]) == 0)
    {
        // printf("%d count\n", count);
        if (count == 3)
        {
            // printf("ici\n");
            res = cd(argv_cpy[1]);
        }
        else
            res = 0;
    }
    else if (is_dot(argv_cpy[0]))
        res = my_dot(argv_cpy, count, env);
    else if (strcmp(argv_cpy[0], "unset") == 0)
        res = my_unset(argv_cpy, env);
    else if (strcmp(argv_cpy[0], "export") == 0)
        res = my_export(argv_cpy, env);
    else
    {
        res = simple_command_exec(argv_cpy, count);
    }
    for (int a = 0; a < count; a++)
    {
        free(argv_cpy[a]);
    }
    free(argv_cpy);
    // printf("res = %d\n", res);
    return res;
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
