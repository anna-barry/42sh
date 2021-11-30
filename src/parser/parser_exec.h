#ifndef PARSER_H
#define PARSER_H

void exec_ast_if_root(struct ast *ast);

void exec_ast_root(struct ast *ast);

void exec_ast_if(struct ast *ast);

void exec_ast_elif(struct ast *ast);

void exec_ast_else(struct ast *ast);

int exec_ast_command(struct ast *ast);

typedef void (*ast_exec_function)(struct ast *ast);

void exec_ast(struct ast *ast);

void execution(struct ast_main_root *ast);

#endif
