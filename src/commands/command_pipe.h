#ifndef COMMAND_PIPE_H
#define COMMAND_PIPE_H

#include "../parser/parser.h"
#include "../parser/parser_exec.h"
#include "command.h"
#include "echo.h"

int pipe_ast(struct ast *ast, struct environnement *env);

#endif
