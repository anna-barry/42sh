#include "get_to_parse.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>

//####################################################################
// GET args
//####################################################################
// en gros juste ca demande des nouvelles données quoi
/*void ask_entry(struct info_lexer *i_lex)
{
    write(1, ">", 1);
    char buf[500];
    scanf("%499[^\n]", buf);
    clear_info(i_lex);
    lexer_new(buf, i_lex);
}*/

//gets the type of redir of chars if existing, else does nothing
int get_opt(struct lexer *lex, struct ast_redir *new)
{
    enum token_type type = lexer_peek(lex)->type;
    if (!lex || type == TOKEN_EOF)
        return 0;
    if (type == TOKEN_REDIR_ENTREE)
        new->opt = REDIR_ENTREE; // <
    else if (type == TOKEN_REDIR_DESCRIPEUR)
        new->opt = REDIR_DESCRIPEUR; //>&
    else if (type == TOKEN_REDIR_SORTIE)
        new->opt = REDIR_SORTIE; // >
    else if (type == TOKEN_REDIR_FIN_FICHIER)
        new->opt = REDIR_FIN_FICHIER; // >>
    else if (type == TOKEN_REDIR_RW)
        new->opt = REDIR_RW; //<>
    else if (type == TOKEN_REDIR_INPUT_DESCRIPEUR)
        new->opt = REDIR_INPUT_DESCRIPEUR; //<&
    else if (type == TOKEN_REDIR_PIPE) //<|
        new->opt = REDIR_PIPE;
    else
        return 1;
    //printf("\n\n\nget opt type = %d\n", type);
    new->redir =
        strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
    return 0;
}
// build the command with conditions and handles error
// returns the number of command generated or -1 if error
int get_command(struct info_lexer *i_lex, struct ast_command *new)
{
    struct lexer *lex = i_lex->lexer;
    enum token_type type = lexer_peek(lex)->type;
    /*if (!lex || type == TOKEN_EOF)
        ask_entry(i_lex);*/
    type = lexer_peek(lex)->type;
    int capy = 30;
    new->argv = malloc(sizeof(char *) * 30);
    new->type = malloc(sizeof(enum ast_type) * 30);
    int y = 0;
    for (; lex && (type == TOKEN_WORDS || type == TOKEN_FOR_WORD || type == TOKEN_SIMPLE_QUOTE || type == TOKEN_FOR_SINGLE_QUOTE || type == TOKEN_DOUBLE_QUOTE || type == TOKEN_FOR_DOUBLE_QUOTE); y++)
    {
        if (y >= capy)
        {
            capy += 30;
            new->argv = realloc(new->argv, capy);
            new->type = realloc(new->type, capy);
        }
        new->argv[y] =
        strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
        new->type[y] = type;
        token_free(lexer_pop(lex));
        type = lexer_peek(lex)->type;
    }
    new->count = y;
    return 0;
}

// create the new node and calls build_ast for the struct
struct ast *get_then(struct info_lexer *i_lex, enum ast_type mode)
{
    struct ast *new  = build_ast(i_lex, mode);
    return new;
}

//when a pipe is found: replaces child by a pipe and puts child as the 
//left child of root
//right is the next args
void get_pipe(struct ast_main_root *ast, struct info_lexer *i_lex)
{
    struct ast_pipe *new_pipe = create_pipe();
    struct lexer *lex = i_lex->lexer;
    ast->nb_children--;
    new_pipe->left->type = ast->children[ast->nb_children - 1]->type;
    new_pipe->left->data = ast->children[ast->nb_children - 1]->data;
    token_free(lexer_pop(lex));
    if ((new_pipe->right = get_then(i_lex, NODE_PIPE)) == NULL)
        errx(2, "wrong pipe implementation");
    ast->children[ast->nb_children - 1]->type = NODE_PIPE;
    ast->children[ast->nb_children - 1]->data.ast_pipe = new_pipe;
}

//get redir
void get_redir(struct ast_main_root *ast, struct info_lexer *i_lex)
{
    struct ast_redir *new = malloc(sizeof(struct ast_redir));
    struct lexer *lex = i_lex->lexer;
    ast->nb_children--;
    //new->command = malloc(sizeof(struct ast));
    new->command->type = ast->children[ast->nb_children - 1]->type;
    new->command->data = ast->children[ast->nb_children - 1]->data;
    get_opt(lex, new);
    token_free(lexer_pop(lex));
    ast->children[ast->nb_children - 1]->type = NODE_REDIR;
    ast->children[ast->nb_children - 1]->data.ast_redir = new;
}

//same as pipe with and
void get_and(struct ast *ast, struct info_lexer *i_lex, enum ast_type mode)
{
    struct ast_and *new_and = create_and();
    struct lexer *lex = i_lex->lexer;
    new_and->left->type = ast->type;
    new_and->left->data = ast->data;
    token_free(lexer_pop(lex));
    if ((new_and->right = get_then(i_lex, mode)) == NULL)
        errx(2, "wrong && implementation");
    ast->type = NODE_AND;
    ast->data.ast_and = new_and;
}

//same as or with or
void get_or(struct ast *ast, struct info_lexer *i_lex, enum ast_type mode)
{
    struct ast_or *new_or = create_or();
    struct lexer *lex = i_lex->lexer;
    new_or->left->type = ast->type;
    new_or->left->data = ast->data;
    token_free(lexer_pop(lex));
    if ((new_or->right = get_then(i_lex, mode)) == NULL)
        errx(2, "wrong && implementation");
    ast->type = NODE_OR;
    ast->data.ast_or = new_or;
}
