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
    NODE_OR,//11
    NODE_AND,//12
    NODE_WHILE,//13
    NODE_DO,//14
    NODE_FOR,
    NODE_FOR_INT,
    NODE_FOR_CHAR,
};

enum opt
{
    NONE,
    REDIR_SORTIE, ///< '>' -> 11
    REDIR_ENTREE, ///< '<' -> 12
    REDIR_DESCRIPEUR, ///< '>&' -> 13
    REDIR_FIN_FICHIER,
    REDIR_INPUT_DESCRIPEUR,
    REDIR_PIPE,
    REDIR_RW,
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
    struct ast_and *ast_and;
    struct ast_or *ast_or;
    struct ast_neg *ast_neg;
    struct ast_for *ast_for;
    struct ast_for_char* ast_for_char;
    struct read_for_int* ast_for_int;
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
    enum opt opt;
    char *redir;
};

struct ast_neg
{
  struct ast *node;
};

struct ast_and
{
  struct ast *right;
  struct ast *left;
};

struct ast_or
{
  struct ast *right;
  struct ast *left;
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

struct ast_for
{
    char *var;
    struct ast *cond; //could be either a list of command //ast_for_word : commands
                // either a struct //read_for_int [1..2..10] : begin, start, step
    struct ast *then;
};

struct ast_for_char
{
    struct ast *var;
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
void ask_entry(struct info_lexer *i_lex);
void get_pipe(struct ast_main_root *ast, struct info_lexer *i_lex);
void get_and(struct ast *ast, struct info_lexer *i_lex, enum ast_type mode);
void get_or(struct ast *ast, struct info_lexer *i_lex, enum ast_type mode);
int get_command(struct info_lexer *i_lex, struct ast_command *new);
struct ast *get_then(struct info_lexer *i_lex, enum ast_type mode);
int build_if( struct info_lexer *i_lex, struct ast_if_root *root);
int build_else( struct info_lexer *i_lex, struct ast_if_root *root);
int build_elif( struct info_lexer *i_lex, struct ast_if_root *root);
struct ast_if_root *build_ast_if( struct info_lexer *i_lex);
int check_break(enum ast_type mode, enum token_type type);
struct ast *build_ast( struct info_lexer *i_lex, enum ast_type mode);

#endif
