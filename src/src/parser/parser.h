#ifndef PARSER_H
#define PARSER_H

enum ast_type
{
    NODE_IF,
    NODE_ELIF,
    NODE_ELSE,
    NODE_IF_ROOT,
    NODE_ROOT,
    NODE_COMMAND,
};

struct ast
{
    enum ast_type type;
};

struct ast_if_root
{
    int nb_children;
    int status;
    struct ast type;
    union ast_data *children;
};

struct ast_command
{
    int count;
    char **argv;
    struct ast type;
};

struct ast_if
{
    struct ast_command *cond;
    struct ast type;
    struct ast_main_root *then;
};
 
struct ast_else
{
    struct ast type;
    struct ast_main_root *then;
};
 
struct ast_elif
{
    struct ast type;
    struct ast_command *cond;
    struct ast_main_root *then;
};

struct ast_main_root
{
    int nb_children;
    struct ast type;
    union ast_data *children;
};

union ast_data
{
    struct ast_if_root *ast_if_root;
    struct ast_command *ast_command;
    struct ast_if *ast_if;
    struct ast_elif *ast_elif;
    struct ast_else *ast_else;
    struct ast_main_root *ast_main_root;
};

union ast_data *create_node(enum ast_type type);

struct lexer *ask_entry(void);

int get_command(struct lexer *lex, struct ast_command *new);

int get_then(struct lexer *lex, struct ast_main_root *new);

int build_if(struct lexer *lex, struct ast_if_root *root);

int build_else(struct lexer *lex, struct ast_if_root *root);

int build_elif(struct lexer *lex, struct ast_if_root *root);

struct ast_if_root *build_ast_if(struct lexer *lex);

struct ast_main_root *build_ast(struct lexer *lex);

#endif