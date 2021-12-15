#ifndef PARSER_EXEC_H
#define PARSER_EXEC_H

#include <stdio.h>
#include <string.h>

#include "../commands/command.h"
#include "../commands/command_pipe.h"
#include "../commands/command_redir.h"
#include "../exec/concat.h"
#include "../functionnal/functionnal.h"
#include "parser.h"

int exec_ast_redir(struct ast *ast, struct environnement *env);

int exec_ast_and(struct ast *ast, struct environnement *env);

int exec_ast_or(struct ast *ast, struct environnement *env);

int exec_ast_neg(struct ast *ast, struct environnement *env);

int exec_ast_if_root(struct ast *ast, struct environnement *env);

int is_dotf(struct ast *ast);

int exec_ast_root(struct ast *ast, struct environnement *env);

int exec_ast_if(struct ast *ast, struct environnement *env);

int exec_ast_elif(struct ast *ast, struct environnement *env);

int exec_ast_else(struct ast *ast, struct environnement *env);

int exec_ast_command(struct ast *ast, struct environnement *env);

int exec_ast(struct ast *ast, struct environnement *env);

int execution(struct ast *ast, struct environnement *env);

#endif
