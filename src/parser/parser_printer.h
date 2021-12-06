#ifndef PARSER_PRINTER_H
#define PARSER_PRINTER_H

#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"

void print_ast(struct ast *ast);

void print_ast_if_root(struct ast *ast);

void print_ast_root(struct ast *ast);

void print_ast_if(struct ast *ast);

void print_ast_elif(struct ast *ast);

void print_ast_else(struct ast *ast);

void print_ast_command(struct ast *ast);

void print_ast_neg(struct ast *ast);

void print_ast_and(struct ast *ast);

void print_ast_or(struct ast *ast);

void print_ast_pipe(struct ast *ast);

void print_ast_simple_quote(struct ast *ast);

void print_ast_double_quote(struct ast *ast);

void print_ast_while(struct ast *ast);

typedef void (*ast_print_function)(struct ast *ast);

void print_ast(struct ast *ast);

void my_pretty_print(struct ast *ast);

#endif
