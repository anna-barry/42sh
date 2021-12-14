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

#include "../parser/parser.h"
#include "../parser/parser_exec.h"
#include "command.h"
#include "echo.h"

int pipe_ast(struct ast *ast, struct environnement *env)
{
    struct ast_pipe *pipe_ast = ast->data.ast_pipe;
    int fd[2];
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
        exec_ast(pipe_ast->left, env);
    }
    int pid2 = fork();
    if (pid2 < 0)
        return 3;
    if (pid2 == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        exec_ast(pipe_ast->right, env);
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
