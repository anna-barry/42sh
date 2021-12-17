#ifndef MAKE_CHILD_H
#define MAKE_CHILD_H

#include "../lexer/lexer.h"
#include "parser.h"
#include "struct.h"

// enum ast_type mode;
// PROCESS AND ADD CHILD WHEN IF
void make_if(struct ast_main_root *ast, struct info_lexer *i_lex);

void make_for(struct ast_main_root *ast, struct info_lexer *i_lex);

void make_while(struct ast_main_root *ast, struct info_lexer *i_lex, int until);

void make_command(struct ast_main_root *ast, struct info_lexer *i_lex);

void make_simple_quote(struct ast_main_root *ast, struct lexer *lex);

void make_double_quote(struct ast_main_root *ast, struct lexer *lex);

void make_neg(struct ast_main_root *ast, struct info_lexer *i_lex,
              enum ast_type mode);

#endif
