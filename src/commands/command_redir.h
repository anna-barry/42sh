#ifndef COMMAND_REDIR_H
#define COMMAND_REDIR_H

#include "../parser/parser.h"
#include "../parser/parser_exec.h"

int command_redir_r(char *command[], int count, char *file,
                    struct environnement *env);
int ast_redir_r(struct ast *ast, char *file, struct environnement *env);

int command_redir_l(char *command[], int count, char *file,
                    struct environnement *env);
int ast_redir_l(struct ast *ast, char *file, struct environnement *env);

int command_redir_rr(char *command[], int count, char *file,
                     struct environnement *env);
int ast_redir_rr(struct ast *ast, char *file, struct environnement *env);

int command_redir_lr(char *command[], int count, char *file,
                     struct environnement *env);
int ast_redir_lr(struct ast *ast, char *file, struct environnement *env);

int command_redir_l_and(char *command[], char *file);
int command_redir_r_and(char *command[], char *file);

int command_redir_r_pipe(char *command[], int count, char *file,
                         struct environnement *env);
int ast_redir_r_pipe(struct ast *ast, char *file, struct environnement *env);

#endif
