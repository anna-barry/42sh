#ifndef GET_TO_PARSE_H
#define GET_TO_PARSE_H

#include "struct.h"
#include "../lexer/lexer.h"
#include "parser.h"

/*enum ast_type;
struct ast_redir;
struct ast_command;
struct ast_main_root;
struct ast;*/

int get_opt(struct lexer *lex, struct ast_redir *new);

int get_command(struct info_lexer *i_lex, struct ast_command *new);

struct ast *get_then(struct info_lexer *i_lex, enum ast_type mode);

void get_pipe(struct ast_main_root *ast, struct info_lexer *i_lex);

void get_redir(struct ast_main_root *ast, struct info_lexer *i_lex);

void get_and(struct ast *ast, struct info_lexer *i_lex, enum ast_type mode);

void get_or(struct ast *ast, struct info_lexer *i_lex, enum ast_type mode);

#endif
