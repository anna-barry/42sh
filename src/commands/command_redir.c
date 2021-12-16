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

#include "../parser/parser.h"
#include "../parser/parser_exec.h"
#include "command.h"

// redirection entry in the begin of a file
// int command_redir_r(struct ast *ast, int count, char *file,
//                     struct environnement *env)
// {
//     int old_fd = dup(STDOUT_FILENO);
//     int fd = open(file, O_CREAT | O_WRONLY, 0644);
//     if (dup2(fd, STDOUT_FILENO) == -1)
//         err(2, "Error with dup2\n");
//     if (command_exec(ast, count, env) == 127)
//     {
//         // close(fd);
//         return 127;
//     }
//     else if (command_exec(ast, count, env) != 0)
//     {
//         // close(fd);
//         return 2;
//     }
//     fflush(stdout);
//     if (dup2(old_fd, STDOUT_FILENO) == -1)
//         err(2, "Error with dup2\n");
//     fcntl(old_fd, F_SETFD, FD_CLOEXEC);
//     close(old_fd);
//     // close(fd);
//     return 0;
// }

int ast_redir_r(struct ast *ast, char *file, struct environnement *env)
{
    struct ast_redir *redir_ast = ast->data.ast_redir;
    int old_fd = dup(STDOUT_FILENO);
    int fd = open(file, O_CREAT | O_WRONLY, 0644);
    if (dup2(fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (exec_ast(redir_ast->command, env) != 0)
    {
        close(fd);
        return 127;
    }
    fflush(stdout);
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    close(STDOUT_FILENO);
    close(fd);
    return 0;
}

// redirection exit in the begin of a file
// int command_redir_l(struct ast *ast, int count, char *file,
//                     struct environnement *env)
// {
//     int old_fd = dup(STDOUT_FILENO);
//     int fd = open(file, O_CREAT | O_WRONLY, 0644);
//     if (dup2(fd, STDIN_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     if (command_exec(ast, count, env) == 127)
//     {
//         close(fd);
//         return 127;
//     }
//     else if (command_exec(ast, count, env) != 0)
//     {
//         close(fd);
//         return 2;
//     }
//     fflush(stdout);
//     if (dup2(old_fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     fcntl(old_fd, F_SETFD, FD_CLOEXEC);
//     close(old_fd);
//     close(fd);
//     return 0;
// }

int ast_redir_l(struct ast *ast, char *file, struct environnement *env)
{
    struct ast_redir *redir_ast = ast->data.ast_redir;
    int old_fd = dup(STDOUT_FILENO);
    int fd = open(file, O_CREAT | O_WRONLY, 0644);
    if (dup2(fd, STDIN_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (exec_ast(redir_ast->command, env) != 0)
    {
        close(fd);
        return 2;
    }
    fflush(stdout);
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    close(STDOUT_FILENO);
    close(fd);
    return 0;
}

// append at the end of the file
<<<<<<< HEAD
// int command_redir_rr(struct ast *ast, int count, char *file,
//                      struct environnement *env)
// {
//     // struct ast_redir *redir_ast = ast->data.ast_redir;
//     // char **command = redir_ast->redir;
//     int old_fd = dup(STDOUT_FILENO);
//     int fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
//     if (dup2(fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     if (command_exec(ast, count, env) == 127)
//     {
//         close(fd);
//         return 127;
//     }
//     else if (command_exec(ast, count, env) != 0)
//     {
//         close(fd);
//         return 2;
//     }
//     fflush(stdout);
//     if (dup2(old_fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     fcntl(old_fd, F_SETFD, FD_CLOEXEC);
//     close(old_fd);
//     close(fd);
//     return 0;
// }
=======
int command_redir_rr(struct ast *ast, int count, char *file,
                     struct environnement *env)
{
    // struct ast_redir *redir_ast = ast->data.ast_redir;
    // char **command = redir_ast->redir;
    int old_fd = dup(STDOUT_FILENO);
    int fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
    if (dup2(fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (command_exec(ast, count, env) == 127)
    {
        close(fd);
        return 127;
    }
    else if (command_exec(ast, count, env) != 0)
    {
        close(fd);
        return 2;
    }
    fflush(stdout);
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    close(STDOUT_FILENO);
    close(fd);
    return 0;
}
>>>>>>> a571c432e0649398c012e39b96d5ec71b46bb929

int ast_redir_rr(struct ast *ast, char *file, struct environnement *env)
{
    struct ast_redir *redir_ast = ast->data.ast_redir;
    int old_fd = dup(STDOUT_FILENO);
    int fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
    if (dup2(fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (exec_ast(redir_ast->command, env) != 0)
    {
        close(fd);
        return 2;
    }
    fflush(stdout);
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    close(old_fd);
    close(fd);
    return 0;
}

// int command_redir_lr(struct ast *ast, int count, char *file,
//                      struct environnement *env)
// {
//     int old_fd = dup(STDOUT_FILENO);
//     int fd = open(file, O_CREAT | O_RDWR, 0644);
//     if (dup2(fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     if (command_exec(ast, count, env) == 127)
//     {
//         close(fd);
//         return 127;
//     }
//     else if (command_exec(ast, count, env) != 0)
//     {
//         close(fd);
//         return 2;
//     }
//     fflush(stdout);
//     if (dup2(old_fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     fcntl(old_fd, F_SETFD, FD_CLOEXEC);
//     close(old_fd);
//     close(fd);
//     return 0;
// }

int ast_redir_lr(struct ast *ast, char *file, struct environnement *env)
{
    struct ast_redir *redir_ast = ast->data.ast_redir;
    int old_fd = dup(STDOUT_FILENO);
    int fd = open(file, O_CREAT | O_RDWR, 0644);
    if (dup2(fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (exec_ast(redir_ast->command, env) != 0)
    {
        close(fd);
        return 2;
    }
    fflush(stdout);
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    close(old_fd);
    close(fd);
    return 0;
}

// dup in and out, basic case 0 for out if NULL
int command_redir_l_and(char *command[], char *file)
{
    int start = 0;
    int i = 0;
    while (command[0][i] != '\0')
    {
        start = start * 10 + ((int)command[0][i] - 48);
        i++;
    }
    int out = start; /*n*/
    start = 0;
    i = 0;
    while (file[i] != '\0')
    {
        start = start * 10 + ((int)file[i] - 48);
        i++;
    }
    int in = start; /*word*/
    int old_fd = dup(STDOUT_FILENO);
    if (dup2(in, out) == -1)
        fprintf(stderr, "Error with dup2");
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    close(old_fd);
    return 0;
}

// dup in and out, basic case 1 for out if NULL
int command_redir_r_and(char *command[], char *file)
{
    int start = 0;
    int i = 0;
    while (command[0][i] != '\0')
    {
        start = start * 10 + ((int)command[0][i] - 48);
        i++;
    }
    int out = start; /*n*/
    start = 0;
    i = 0;
    while (file[i] != '\0')
    {
        start = start * 10 + ((int)file[i] - 48);
        i++;
    }
    int in = start; /*word*/
    int old_fd = dup(STDOUT_FILENO);
    if (dup2(in, out) == -1)
        fprintf(stderr, "Error with dup2");
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    close(old_fd);
    return 0;
}

// prevent from overwriting on a file if already existing
// int command_redir_r_pipe(struct ast *ast, int count, char *file,
//                          struct environnement *env)
// {
//     int fd = 0;
//     if ((fd = open(file, O_RDONLY, 0644)) >= 0)
//         err(2, "the file %s already exist, you can not overwrite it", file);
//     int old_fd = dup(STDOUT_FILENO);
//     fd = open(file, O_CREAT | O_WRONLY, 0644);
//     if (dup2(fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     if (command_exec(ast, count, env) == 127)
//     {
//         close(fd);
//         return 127;
//     }
//     else if (command_exec(ast, count, env) != 0)
//     {
//         close(fd);
//         return 2;
//     }
//     fflush(stdout);
//     if (dup2(old_fd, STDOUT_FILENO) == -1)
//         fprintf(stderr, "Error with dup2");
//     fcntl(old_fd, F_SETFD, FD_CLOEXEC);
//     close(old_fd);
//     close(fd);
//     return 0;
// }

int ast_redir_r_pipe(struct ast *ast, char *file, struct environnement *env)
{
    int fd = 0;
    struct ast_redir *redir_ast = ast->data.ast_redir;
    if ((fd = open(file, O_RDONLY, 0644)) >= 0)
        err(2, "the file %s already exist, you can not overwrite it", file);
    int old_fd = dup(STDOUT_FILENO);
    fd = open(file, O_CREAT | O_WRONLY, 0644);
    if (dup2(fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    if (exec_ast(redir_ast->command, env) != 0)
    {
        close(fd);
        return 2;
    }
    fflush(stdout);
    if (dup2(old_fd, STDOUT_FILENO) == -1)
        fprintf(stderr, "Error with dup2");
    fcntl(old_fd, F_SETFD, FD_CLOEXEC);
    close(old_fd);
    close(fd);
    return 0;
}

// int main()
// {
//     char *argv[2] = { "echo", "geoffroy" };
//     char *file = "tester.txt";
//     // printf("recieve\n");
//     // fprintf(fd, "new file descriptor\n");
//     command_redir_r(argv, 2, file);
//     // command_redir_r_pipe(argv, 2, file);
//     // fprintf(fd, "old file descriptor\n");
//     // printf("lounch\n");
//     return 0;
// }
