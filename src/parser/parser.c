#include "parser.h"

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"

struct ast_main_root *build_ast(struct lexer *lex);
//creating and allocating space for node
//type is here to determine wich type of struct has to be created
union ast_data *create_node(enum ast_type type)
{
    union ast_data *new;
    if (type == NODE_ROOT)
    {
        struct ast_main_root *ast = malloc(sizeof(struct ast_main_root));
        ast->nb_children = 0;
        ast->children = malloc(sizeof(union ast_data));
        ast->type.type = NODE_ROOT;
        new->ast_main_root = ast;
    }
    if (type == NODE_IF_ROOT)
    {
        struct ast_if_root *root = malloc(sizeof(struct ast_if_root));
        root->type.type = NODE_IF_ROOT;
        root->nb_children = 0;
        root->status = 0;
        root->children = malloc(sizeof(struct ast_if));
        new->ast_if_root = root;
    }
    else if (type == NODE_ELSE)
    {
        struct ast_else *new_else = malloc(sizeof(struct ast_else));
        new_else->type.type = NODE_ELSE;
        new_else->then = malloc(sizeof(struct ast_command));
        new->ast_else = new_else;
    }
    else if (type == NODE_IF)
    {
        struct ast_if *new_if = malloc(sizeof(struct ast_if));
        new_if->type.type = NODE_IF;
        new_if->cond = malloc(sizeof(struct ast_command));
        new_if->then = malloc(sizeof(union ast_data));
        new->ast_if = new_if;
    }
    else if (type == NODE_ELIF)
    {
        struct ast_elif *new_elif = malloc(sizeof(struct ast_elif));
        new_elif->type.type = NODE_ELIF;
        new_elif->cond = malloc(sizeof(struct ast_command));
        new_elif->then = malloc(sizeof(union ast_data));
        new->ast_elif = new_elif;
    }
    struct ast_command *new_c = malloc(sizeof(struct ast_command));
    new_c->type.type = NODE_COMMAND;
    new_c->count = 0;
    new->ast_command = new_c;
    return new;
}

//en gros juste ca demande des nouvelles données quoi
struct lexer *ask_entry(void)
{
    write(1, ">", 1);
    char buf[500]; //chiffre basic jsp mettre on testera
    scanf("%s", buf);
    return lexer_new(buf);
}

//build the char with conditions and handles error
int get_command(struct lexer *lex, struct ast_command *new)
{
    if (!lex)
        lex = ask_entry();
    if (lexer_peek(lex)->type != TOKEN_WORDS)
        return 1;
    int i = 0;
    for (; lex && lexer_peek(lex)->type == TOKEN_WORDS; i++)
    {
        new = realloc(new, sizeof(struct ast_command) * (i +1));
        new[i].argv = malloc(sizeof (char *));
        int y = 0;
        for (; lex && lexer_peek(lex)->type == TOKEN_WORDS; y++)
        {
            new[i].argv = realloc(new->argv, y + 1);
            new[i].argv[y] = lexer_peek(lex)->value;
            lexer_pop(lex);
        }
        new[i].argv = realloc(new[i].argv, y + 1);
        new[i].argv[y] = NULL;
        if (!lex)
            lex = ask_entry();
        if (lex && lexer_peek(lex)->type == TOKEN_SEMICOLON)
            lexer_pop(lex);
    }
    new = realloc(new, ++i);
    new->count = i;
    new[i].argv = NULL;
    return 0;
}

int get_then(struct lexer *lex, struct ast_main_root *new)
{
    if (!lex)
        lex = ask_entry();
    if (lexer_peek(lex)->type != TOKEN_THEN)
        return 1;
    new = build_ast(lex);
    return 0;
}

//returns 0 if ok, if error 1
//builds the if ast with cond ast and then ast
int build_if(struct lexer *lex, struct ast_if_root *root)
{
    struct ast_if *new_if = create_node(NODE_IF)->ast_if;
    
    if (get_command(lex, new_if->cond) || get_then(lex, new_if->then)) //if one of them is an error, then 1
        return 1;

    root->nb_children = 1;
    root->children[0].ast_if = new_if;
    return 0;
}

int build_else(struct lexer *lex, struct ast_if_root *root)
{
    struct ast_else *new_else = create_node(NODE_ELSE)->ast_else;
    
    if (get_then(lex, new_else->then))
        return 1;
    
    root->nb_children++;
    root->children = realloc(root->children, sizeof(struct ast_if) * root->nb_children);
    root->children[root->nb_children - 1].ast_else = new_else;
    return 0;
}

int build_elif(struct lexer *lex, struct ast_if_root *root)
{
    struct ast_elif *new_elif = create_node(NODE_ELIF)->ast_elif;
    if (get_command(lex, new_elif->cond) || get_then(lex, new_elif->then)) //if one of them is an error, then 1
        return 1;
    
    int size = sizeof(struct ast_if) * root->nb_children + sizeof(struct ast_else);
    root->children = realloc(root->children, size);
    root->children[root->nb_children].ast_elif = new_elif;
    root->children++;
    return 0;
}

//builds and return ast_if_root if command is if
struct ast_if_root *build_ast_if(struct lexer *lex)
{
    //creating the ast_if_root node
    struct ast_if_root *new_root = create_node(NODE_ROOT)->ast_if_root;

    //here getting if out of the lexer
    struct token *tip = lexer_pop(lex);

    //add if
    if (build_if(lex, new_root))
        errx(2, "bad args in a wrong place in IF");
    
    while (!lex)
        lex = ask_entry();

    if (lexer_peek(lex)->type != TOKEN_ELSE && lexer_peek(lex)->type != TOKEN_ELIF && lexer_peek(lex)->type != TOKEN_FI)
        errx(2, "bad args in a wrong place after if");
    //add elif childs
    while ((tip = lexer_pop(lex)) != NULL && tip->type == TOKEN_ELIF)
    {
        if (build_elif(lex, new_root))
            errx(2, "bad args in a wrong place in ELIF");
        while (!lex)
            lex = ask_entry();
    }
    if (lexer_peek(lex)->type != TOKEN_ELSE && lexer_peek(lex)->type != TOKEN_FI)
        errx(2, "bad args in a wrong place after elif");
    //add else childs if existing
    if (lexer_peek(lex)->type == TOKEN_ELSE)
        build_else(lex, new_root);
    
    while (!lex)
        lex = ask_entry();
    if (lexer_peek(lex)->type !=  TOKEN_FI)
        errx(2, "needed FI to close de IF condition");
    
    //remove the FI token
    lexer_pop(lex);
    return new_root;
}

//small begining of creation of global ast
//char is already lexed so need a function before to link everything
//for now only handles IF and commands (for now echo)
struct ast_main_root *build_ast(struct lexer *lex)
{
    struct ast_main_root *ast = create_node(NODE_IF_ROOT)->ast_main_root;
    if (!lex)
        lex = ask_entry();
    while (lex)
    {
        ast->nb_children++;
        ast->children = realloc(ast->children, sizeof(union ast_data) * ast->nb_children);
        if (lexer_peek(lex)->type == TOKEN_IF)
            ast->children[ast->nb_children - 1].ast_if_root = build_ast_if(lex);
        if (lexer_peek(lex)->type == TOKEN_WORDS)
        {
            struct ast_command *new_com = create_node(NODE_COMMAND)->ast_command;
            if (get_command(lex, new_com))
                errx(2, "could'nt get condition");
            ast->children[ast->nb_children - 1].ast_command->type.type = NODE_COMMAND;
            ast->children[ast->nb_children - 1].ast_command = new_com;
        }
    }
    return ast;
}
