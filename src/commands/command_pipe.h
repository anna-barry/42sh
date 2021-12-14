#ifndef COMMAND_PIPE_H
#define COMMAND_PIPE_H

int pipe_ast(struct ast *ast, struct environnement *env);

int pipe_exec(char *argv_left[], char *argv_right[], int count_left,
              int count_right);

#endif
