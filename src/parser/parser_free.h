#ifndef PARSER_FREE_H
#define PARSER_FREE_H

#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "parser.h"

void free_ast(struct ast *ast);

void free_ast_if_root(struct ast *ast);

void free_ast_root(struct ast *ast);

void free_ast_if(struct ast *ast);

void free_ast_elif(struct ast *ast);

void free_ast_else(struct ast *ast);

void free_ast_command();

void free_ast_neg(struct ast *ast);

void free_ast_and(struct ast *ast);

void free_ast_or(struct ast *ast);

void free_ast_pipe(struct ast *ast);

void free_ast_simple_quote();

void free_ast_double_quote();

void free_ast_while(struct ast *ast);

void free_ast_for(struct ast *ast);

void free_ast_for_char(struct ast *ast);

void free_ast_for_int();

typedef void (*ast_free_function)(struct ast *ast);

void free_ast(struct ast *ast);

void my_pretty_free(struct ast *ast);

#endif
