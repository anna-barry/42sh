#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "struct_init.h"
#include "get_to_parse.h"
#include "make_child.h"
#include "struct.h"

//void ask_entry(struct info_lexer *i_lex);
int build_if( struct info_lexer *i_lex, struct ast_if_root *root);
int build_else( struct info_lexer *i_lex, struct ast_if_root *root);
int build_elif( struct info_lexer *i_lex, struct ast_if_root *root);
struct ast_if_root *build_ast_if( struct info_lexer *i_lex);
struct ast_for *build_ast_for(struct info_lexer *i_lex);
struct ast_while *build_ast_while(struct info_lexer *i_lex, int until);
int check_break(enum ast_type mode, enum token_type type, int open);
struct ast *build_ast( struct info_lexer *i_lex, enum ast_type mode);

#endif
