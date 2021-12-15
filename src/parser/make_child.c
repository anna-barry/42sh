#include "make_child.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>

// PROCESS AND ADD CHILD WHEN IF
void make_if(struct ast_main_root *ast, struct info_lexer *i_lex)
{
    ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
    ast->children[ast->nb_children - 1]->type = NODE_IF_ROOT;
    ast->children[ast->nb_children - 1]->data.ast_if_root = build_ast_if(i_lex);
}

void make_for(struct ast_main_root *ast, struct info_lexer *i_lex)
{
    ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
    ast->children[ast->nb_children - 1]->type = NODE_FOR;
    ast->children[ast->nb_children - 1]->data.ast_for = build_ast_for(i_lex);
}

//PROCESS AND ADD CHILD WHEN while
void make_while(struct ast_main_root *ast, struct info_lexer *i_lex, int until)
{
    token_free(lexer_pop(i_lex->lexer));
    ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
    ast->children[ast->nb_children - 1]->type = NODE_WHILE;
    ast->children[ast->nb_children - 1]->data.ast_while = build_ast_while(i_lex, until);
}

// PROCESS AND ADD CHILD WHEN COMMAND
void make_command(struct ast_main_root *ast, struct info_lexer *i_lex)
{
    struct ast_command *new_com = create_command();
    if (get_command(i_lex, new_com))
        errx(2, "couldn't get condition");
    int rank = ast->nb_children - 1;
    ast->children[rank] = malloc(sizeof(struct ast));
    // to be tested
    ast->children[rank]->type = NODE_COMMAND;
    ast->children[rank]->data.ast_command = new_com;
    //printf("\n\n\n\nTOKEN THEN\n\n\n\n");
    /*if (lexer_peek(lex)->type == TOKEN_AND)
        get_and(ast->children[rank], i_lex, mode);
    else if (lexer_peek(lex)->type == TOKEN_OR)
        get_or(ast->children[rank], i_lex, mode);*/
}

// make single quote
void make_simple_quote(struct ast_main_root *ast, struct lexer *lex)
{
    int rank = ast->nb_children - 1;
    ast->children[rank] = malloc(sizeof(struct ast));
    ast->children[rank]->type = NODE_SIMPLE_QUOTE;
    ast->children[rank]->data.ast_simple_quote =
        malloc(sizeof(struct ast_simple_quote));
    ast->children[rank]->data.ast_simple_quote->argv =
        strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
    token_free(lexer_pop(lex));
}

void make_double_quote(struct ast_main_root *ast, struct lexer *lex)
{
    int rank = ast->nb_children - 1;
    ast->children[rank] = malloc(sizeof(struct ast));
    ast->children[rank]->type = NODE_DOUBLE_QUOTE;
    ast->children[rank]->data.ast_double_quote =
        malloc(sizeof(struct ast_double_quote));
    ast->children[rank]->data.ast_double_quote->argv =
        strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
    token_free(lexer_pop(lex));
}

void make_neg(struct ast_main_root *ast, struct info_lexer *i_lex, enum ast_type mode)
{
    int rank = ast->nb_children - 1;
    ast->children[rank] = malloc(sizeof(struct ast));
    token_free(lexer_pop(i_lex->lexer));
    ast->children[rank]->type = NODE_NEG;
    ast->children[rank]->data.ast_neg = malloc(sizeof(struct ast_neg));
    //ast->children[rank]->data.ast_neg->node = malloc(sizeof(struct ast));
    ast->children[rank]->data.ast_neg->node =  build_ast(i_lex, mode);
}
