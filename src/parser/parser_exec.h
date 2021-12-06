#ifndef PARSER_EXEC_H
#define PARSER_EXEC_H

#include <stdio.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/command_pipe.h"
#include "../commands/command_redir.h"
#include "parser.h"

int exec_ast_if_root(struct ast *ast);

int exec_ast_root(struct ast *ast);

int exec_ast_if(struct ast *ast);

int exec_ast_elif(struct ast *ast);

int  exec_ast_else(struct ast *ast);

int exec_ast_command(struct ast *ast);

typedef int (*ast_exec_function)(struct ast *ast);

int exec_ast(struct ast *ast);

int execution(struct ast *ast);

#endif
