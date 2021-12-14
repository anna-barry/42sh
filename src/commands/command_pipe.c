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
    struct ast_pipe *pipe_ast = ast->data.ast_pipe;
    int fd[2];
    if (pipe(fd) == -1)
        return 1;
    int fdout = dup(STDOUT_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    int res = exec_ast(pipe_ast->left, env);
    dup2(fdout, STDOUT_FILENO);
    close(fdout);
    close(fd[1]);
    int fdin = dup(STDIN_FILENO);
    dup2(fd[0], STDIN_FILENO);
    res = exec_ast(pipe_ast->right, env);
    dup2(fdin, STDIN_FILENO);
    close(fdin);
    close(fd[0]);
    return res;
}
