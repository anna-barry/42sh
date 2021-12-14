#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../parser/parser.h"
#include "../parser/parser_exec.h"
#include "command.h"
#include "echo.h"

int pipe_ast(struct ast *ast, struct environnement *env)
{
    printf("in pipe \n");
    fflush(stdout);
    struct ast_pipe *pipe_ast = ast->data.ast_pipe;
    int fd[2];
    // printf("ici\n");
    // fflush(stdout);
    if (pipe(fd) == -1)
        return 1;
    int pid = fork();
    if (pid < 0)
    {
        return 1;
    }
    if (pid == 0)
    {
        // close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        exec_ast(pipe_ast->left, env);
        // printf("ici\n");
        // fflush(stdout);
    }
    int pid2 = fork();
    if (pid2 < 0)
        return 3;
    if (pid2 == 0)
    {
        // close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        // printf("before child second\n");
        // fflush(stdout);
        exec_ast(pipe_ast->right, env);
        // printf("child second\n");
        // fflush(stdout);
        // putchar('a');
    }
    else
    {
        close(fd[0]);
        close(fd[1]);
        // putchar('a');
        // printf("lalalalala\n");
        // fflush(stdout);
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
        // printf("la pute de w\n");
        // fflush(stdout);
        int wstatus;
        kill(pid2, SIGKILL);
        waitpid(pid2, &wstatus, 0);
        // printf("la pute de sa grosse race\n");
        // fflush(stdout);
        if (WEXITSTATUS(wstatus) == 2 || WEXITSTATUS(wstatus) == 0)
            return WEXITSTATUS(wstatus);
        return 1;
    }
    return 0;
}
