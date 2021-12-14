#ifndef CONCAT_H
#define CONCAT_H

#include <stdio.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/command_pipe.h"
#include "../commands/command_redir.h"
#include "../functionnal/functionnal.h"
#include "../parser/parser.h"

char *itoa(int value, char *s);

char *transform_char(char *argv, struct environnement *env, int *index);

void transform_command(struct ast *ast, struct environnement *env);

void concat_node(struct ast *node1, struct ast *node2,
                 struct environnement *env);

void concat_command(struct ast_main_root *a, int *i, struct environnement *env);

#endif
