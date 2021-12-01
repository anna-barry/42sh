#include "parser.h"

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//####################################################################
//INITIALIZER OF EVERY STRUCT
//####################################################################
struct ast_main_root *create_main_root()
{
    struct ast_main_root *ast = malloc(sizeof(struct ast_main_root));
    ast->nb_children = 0;
    ast->children = malloc(sizeof(struct ast *));
    return ast;
}

struct ast_if_root *create_if_root()
{
    struct ast_if_root *root = malloc(sizeof(struct ast_if_root));
    root->nb_children = 0;
    root->status = 1;
    root->children = malloc(sizeof(struct ast));
    return root;
}

struct ast_else *create_else()
{
    struct ast_else *new_else = malloc(sizeof(struct ast_else));
    new_else->then = malloc(sizeof(struct ast));
    return new_else;
}

struct ast_if *create_if()
{
    struct ast_if *new_if = malloc(sizeof(struct ast_if));
    new_if->count_cond = 0;
    new_if->then = malloc(sizeof(struct ast));
    return new_if;
}

struct ast_command **create_command()
{
    struct ast_command **new_c = malloc(sizeof(struct ast_command *) * 30);
    new_c[0] = malloc(sizeof(struct ast_command) * 30);
    new_c[0]->count = 0;
    return new_c;
}
struct ast_elif *create_elif()
{
    struct ast_elif *new_elif = malloc(sizeof(struct ast_elif));
    new_elif->count_cond = 0;
    new_elif->then = malloc(sizeof(struct ast));
    return new_elif;
}
//en gros juste ca demande des nouvelles données quoi
struct lexer *ask_entry(void)
{
    write(1, ">", 1);
    char buf[500]; //chiffre basic jsp mettre on testera
    scanf("%s", buf);
    return lexer_new(buf);
}

//####################################################################
//GET args
//####################################################################

//build the command with conditions and handles error
int get_command(struct lexer *lex, struct ast_command **new)
{
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    if (lexer_peek(lex)->type != TOKEN_WORDS && lexer_peek(lex)->type != TOKEN_SIMPLE_QUOTE)
        return -1;
    int capy = 30;
    int capi = 30;
    int i = 0;
    for (; lex && ((lexer_peek(lex)->type == TOKEN_WORDS || lexer_peek(lex)->type == TOKEN_SIMPLE_QUOTE)); i++)
    {
        if (i != 0)
            new[i] = malloc(sizeof(struct ast_command));
        if (i >= capi)
        {
            capi *= 2;
            new = realloc(new, sizeof(struct ast_command *) * capi);
        }
        new[i]->argv = malloc(sizeof (char *) * 30);
        int y = 0;
        for (; lex && (lexer_peek(lex)->type == TOKEN_WORDS || lexer_peek(lex)->type == TOKEN_SIMPLE_QUOTE); y++)
        {
            if (y >= capy)
            {
                capy *= 2;
                new[i]->argv = realloc(new[i]->argv, capy);
            }
            new[i]->argv[y] = strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
            lexer_pop(lex);
        }
        new[i]->count = y;
        if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
            lex = ask_entry();
        if (lex && lexer_peek(lex)->type == TOKEN_SEMICOLON)
            lexer_pop(lex);
    }
    return i;
}

//create the new node and calls build_ast for the struct
int get_then(struct lexer *lex, struct ast *new, enum ast_type mode)
{
    new->type = NODE_ROOT;
    if (!lex  || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    if (lexer_peek(lex)->type != TOKEN_WORDS)
        return 1;
    new->data.ast_main_root = build_ast(lex, mode)->data.ast_main_root;
    return 0;
}

//####################################################################
//BUILDING EACH STRUCTURE HANDLING ERRORS
//####################################################################
//returns 0 if ok, if error 1
//builds the if ast with cond ast and then ast
int build_if(struct lexer *lex, struct ast_if_root *root)
{
    struct ast_if *new_if = create_if();
    struct ast_command **tmp = create_command();
    int test = get_command(lex, tmp);
    new_if->cond = malloc(sizeof(struct ast *) * test);
    for (int i = 0; i < test; i++)
    {
        new_if->cond[i] = malloc(sizeof(struct ast));
        new_if->cond[i]->type = NODE_COMMAND;
        new_if->cond[i]->data.ast_command = tmp[i];
    }
    if (lexer_peek(lex)->type != TOKEN_THEN)
        return 1;
    lexer_pop(lex);
    if (test == -1 || get_then(lex, new_if->then, NODE_IF)) //if one of them is an error, then 1
        return 1;
    new_if->count_cond = test;
    root->nb_children = 1;
    root->children[0] = malloc(sizeof(struct ast));
    root->children[0]->type = NODE_IF;
    root->children[0]->data.ast_if = new_if;
    return 0;
}

int build_else(struct lexer *lex, struct ast_if_root *root)
{
    struct ast_else *new_else = create_else();
    lexer_pop(lex);
    if (get_then(lex, new_else->then, NODE_ELSE))
        return 1;
    //root->children = realloc(root->children, sizeof(struct ast) * root->nb_children);
    root->children[root->nb_children] = malloc(sizeof(struct ast));
    root->children[root->nb_children]->type = NODE_ELSE;
    root->children[root->nb_children]->data.ast_else = new_else;
    root->nb_children++;
    return 0;
}

int build_elif(struct lexer *lex, struct ast_if_root *root)
{
    struct ast_elif *new_elif = create_elif();
    struct ast_command **tmp = create_command();
    int test = get_command(lex, tmp);
    new_elif->cond = malloc(sizeof(struct ast *) * test);
    for (int i = 0; i < test; i++)
    {
            new_elif->cond[i] = malloc(sizeof(struct ast));
            new_elif->cond[i]->type = NODE_COMMAND;
            new_elif->cond[i]->data.ast_command = tmp[i];
    }
    if (lexer_peek(lex)->type != TOKEN_THEN)
        return 1;
    lexer_pop(lex);
    if (test == -1 || get_then(lex, new_elif->then, NODE_ELIF)) //if one of them is an error, then 1
        return 1;
    
    int size = sizeof(struct ast) * (root->nb_children + 1);
    new_elif->count_cond = test;
    root->children[root->nb_children] = malloc(sizeof(struct ast));
    root->children[root->nb_children]->type = NODE_ELIF;
    root->children[root->nb_children]->data.ast_elif = new_elif;
    root->nb_children++;
    return 0;
}

//builds global ast_if_root
struct ast_if_root *build_ast_if(struct lexer *lex)
{
    //creating the ast_if_root node
    struct ast_if_root *new_root = create_if_root();
    int cap = 30;
    new_root->children = malloc(sizeof(struct ast_if) * cap);
    //here getting if out of the lexer
    lexer_pop(lex);
    
    //add if
    if (build_if(lex, new_root))
        errx(2, "bad args in a wrong place in IF");
    while (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    
    //checking for errors
    if (lex && lexer_peek(lex)->type != TOKEN_ELSE && lexer_peek(lex)->type != TOKEN_ELIF && lexer_peek(lex)->type != TOKEN_FI)
        errx(2, "bad args in a wrong place after if");
    
    //if/while elif childs are existing adding them
    while (lexer_peek(lex)->type != TOKEN_EOF && lexer_peek(lex)->type == TOKEN_ELIF)
    {
        if (new_root->nb_children >= cap)
        {
            cap *= 2;
            new_root->children = realloc(new_root->children, sizeof(struct ast_if) * cap);
        }
        lexer_pop(lex);
        if (build_elif(lex, new_root))
            errx(2, "bad args in a wrong place in ELIF");
        while (!lex || lexer_peek(lex)->type == TOKEN_EOF)
            lex = ask_entry();
    }
    //checking for errors
    if (lexer_peek(lex)->type != TOKEN_ELSE && lexer_peek(lex)->type != TOKEN_FI)
        errx(2, "bad args in a wrong place after elif");
    
    //add else childs if existing
    if (lexer_peek(lex)->type == TOKEN_ELSE)
    {
        if (new_root->nb_children >= cap)
        {
            cap *= 2;
            new_root->children = realloc(new_root->children, sizeof(struct ast_if) * cap);
        }
        build_else(lex, new_root);
    }
    
    while (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    
    //checking TOKEN FI ends the command
    if (lexer_peek(lex)->type !=  TOKEN_FI)
        errx(2, "needed FI to close de IF condition");
    //remove the FI token
    lexer_pop(lex);
    return new_root;
}

//################################################################
//#####EACH FUNCTION TOOL IS HANDLED HERE TO ADD IN THE TREE######

//PROCESS AND ADD CHILD WHEN IF
void make_if(struct ast_main_root *ast, struct lexer *lex)
{
    ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
    ast->children[ast->nb_children - 1]->type = NODE_IF_ROOT;
    ast->children[ast->nb_children - 1]->data.ast_if_root = build_ast_if(lex);
}

//PROCESS AND ADD CHILD WHEN COMMAND
void make_command(struct ast_main_root *ast, struct lexer *lex, int *count)
{
    struct ast_command **new_com = create_command();
    int test = get_command(lex, new_com);
    if (test == -1)
        errx(2, "couldn't get condition");
    int ind = 0;
    for (; ind < test; ind++)
    {
        ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
        if (ast->nb_children >= *count)
        {
            *count *= 2;
            ast->children = realloc(ast->children, sizeof(struct ast *) * *count);
        }
        ast->children[ast->nb_children - 1]->type = NODE_COMMAND;
        ast->children[ast->nb_children - 1]->data.ast_command = new_com[ind];
        ast->nb_children++;
    }
    if (ind != 0)
        ast->nb_children--;
}

//################################################################
//#######MAIN ROOT STRUCTURE HANDLING FUNCTIONS###################

//DEPEDENING ON THE ROOT OF THE CALL TO BUILD_AST THE BREAK CASE IS DIFFERENT
int check_break(enum ast_type mode, enum token_type type)
{
    if (mode == NODE_ROOT && type == TOKEN_EOF)
        return 0;
    if (mode == NODE_ROOT && type != TOKEN_WORDS && type != TOKEN_IF)
        errx(2, "wrong implementation in node root");
    if (mode == NODE_IF || mode == NODE_ELIF)
    {
        if (type == TOKEN_ELSE || type == TOKEN_ELIF || type == TOKEN_FI)
            return 0;
    }
    if (mode == NODE_ELSE && type != TOKEN_WORDS && type != TOKEN_IF)
        return 0;
    return 1;
}

//small begining of creation of global ast
//char is already lexed so need a function before to link everything
//for now only handles IF and commands (for now echo)
struct ast *build_ast(struct lexer *lex, enum ast_type mode)
{
    struct ast *new_ast = malloc(sizeof(struct ast));
    new_ast->type = NODE_ROOT;
    struct ast_main_root *ast = create_main_root();
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    int count = 30;
    ast->children = malloc(sizeof(struct ast *) * 30);
    while (lex && check_break(mode, lexer_peek(lex)->type))
    {
        ast->nb_children++;
        if (ast->nb_children >= count)
        {
            count *= 2;
            ast->children = realloc(ast->children, sizeof(struct ast *) * count);
        }
        //IF WORD IS IF THEN MAKE IF
        if (lexer_peek(lex)->type == TOKEN_IF)
            make_if(ast, lex);
        //IF WORD IS WORD OR SEMICOLON MAKE COMMAND
        else if (lexer_peek(lex)->type == TOKEN_WORDS || lexer_peek(lex)->type == TOKEN_SEMICOLON)
            make_command(ast, lex, &count);
        else
            errx(2, "wrong implementation");
    }
    new_ast->data.ast_main_root = ast;
    return new_ast;
}
