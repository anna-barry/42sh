#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include "../lexer/token.h"

enum ast_type
{
    NODE_IF, //0
    NODE_ELIF,//1
    NODE_ELSE,//2
    NODE_IF_ROOT,//3
    NODE_ROOT,//4
    NODE_COMMAND,//5
    NODE_SIMPLE_QUOTE,//6
    NODE_DOUBLE_QUOTE,//7
    NODE_THEN,//8
    NODE_NEG,//9
    NODE_PIPE,//10
};

enum option
{
    NONE,
    REDIR_SORTIE, ///< '>' -> 11
    REDIR_ENTREE, ///< '<' -> 12
    REDIR_DESCRIPEUR, ///< '>&' -> 13
    REDIR_FIN_FICHIER,
};

union ast_data
{
    struct ast_if_root *ast_if_root;
    struct ast_command *ast_command;
    struct ast_if *ast_if;
    struct ast_elif *ast_elif;
    struct ast_else *ast_else;
    struct ast_main_root *ast_main_root;
    struct ast_simple_quote *ast_simple_quote;
    struct ast_double_quote *ast_double_quote;
    struct ast_pipe *ast_pipe;
    struct ast_while *ast_while;
};

struct ast
{
    enum ast_type type;
    union ast_data data;
};

struct ast_if_root
{
    int nb_children;
    int status;
    struct ast **children;
};

struct ast_command
{
    int count;
    char **argv;
    enum option option;
    char *redir;
};

struct ast_neg
{
  struct ast *node;
};

struct ast_pipe
{
  struct ast *right;
  struct ast *left;
};

struct ast_simple_quote
{
    char *argv;
};

struct ast_double_quote
{
    char *argv;
};

struct ast_if
{
    struct ast *cond;
    struct ast *then;
};

struct ast_while
{
    struct ast *cond;
    struct ast *then;
};

struct ast_else
{
    struct ast *then;
};

struct ast_elif
{
    struct ast *cond;
    struct ast *then;
};

struct ast_main_root
{
    int nb_children;
    struct ast **children;
};

struct ast_main_root *create_main_root();
struct ast_if_root *create_if_root();
struct ast_else *create_else();
struct ast_if *create_if();
struct ast_command *create_command();
struct ast_elif *create_elif();
struct lexer *ask_entry(void);
int get_command(struct lexer *lex, struct ast_command *new);
int get_then(struct lexer *lex, struct ast *new, enum ast_type mode);
int build_if(struct lexer *lex, struct ast_if_root *root);
int build_else(struct lexer *lex, struct ast_if_root *root);
int build_elif(struct lexer *lex, struct ast_if_root *root);
struct ast_if_root *build_ast_if(struct lexer *lex);
int check_break(enum ast_type mode, enum token_type type);
struct ast *build_ast(struct lexer *lex, enum ast_type mode);

#endif
