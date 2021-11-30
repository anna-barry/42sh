#include "parser.h"

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"

void print(struct lexer *lexer)
{
    printf("START OF PRINTING ##########\n");
    char *tab[] = {
        [TOKEN_IF] = "If", [TOKEN_THEN] = "Then",  [TOKEN_ELIF] = "Elif",
        [TOKEN_ELSE] = "Else",  [TOKEN_FI] = "Fi", [TOKEN_SEMICOLON] = ";",
        [TOKEN_LINE_BREAK] = "\n"
    };
    size_t i = 0;
    if (lexer == NULL || lexer[i].current_tok == NULL)
        return;
    while (lexer[i].current_tok->type != TOKEN_EOF)
        {
            if (lexer[i].current_tok->type == TOKEN_SIMPLE_QUOTE || lexer[i].current_tok->type == TOKEN_WORDS)
                printf("%s\n", lexer[i].current_tok->value);
            else
                printf("%s\n", tab[lexer[i].current_tok->type]);
            i++;
        }
    printf("END OF PRINTING ##########\n");
}
//creating and allocating space for node
//type is here to determine wich type of struct has to be created
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
    struct ast_command **new_c = malloc(sizeof(struct ast_command *));
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

//build the char with conditions and handles error
int get_command(struct lexer *lex, struct ast_command **new)
{
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    if (lexer_peek(lex)->type != TOKEN_WORDS)
        return -1;
    int capy = 30;
    int capi = 30;
    int i = 0;
    //new = malloc(sizeof(struct ast_command *) * capi);
    for (; lex && lexer_peek(lex)->type == TOKEN_WORDS; i++)
    {
        if (i >= capi)
        {
            capi *= 2;
            new = realloc(new, sizeof(struct ast_command *) * capi);
        }
        new[i]->argv = malloc(sizeof (char *) * 30);
        int y = 0;
        for (; lex && lexer_peek(lex)->type == TOKEN_WORDS; y++)
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
        lexer_pop(lex);
        print(lex);
        if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
            lex = ask_entry();
        if (lex && lexer_peek(lex)->type == TOKEN_SEMICOLON)
            lexer_pop(lex);
    }
    //new[i]->argv = NULL;
    printf("TEST\n");
    return i;
}

int get_then(struct lexer *lex, struct ast *new)
{
    new->type = NODE_ROOT;
    if (!lex  || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    if (lexer_peek(lex)->type != TOKEN_THEN)
        return 1;
    print(lex);
    lexer_pop(lex);
    print(lex);
    new->data.ast_main_root = build_ast(lex)->data.ast_main_root;
    return 0;
}

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
    if (test == -1 || get_then(lex, new_if->then)) //if one of them is an error, then 1
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
    
    if (get_then(lex, new_else->then))
        return 1;
    
    root->nb_children++;
    root->children = realloc(root->children, sizeof(struct ast) * root->nb_children);
    root->children[root->nb_children - 1] = malloc(sizeof(struct ast));
    root->children[root->nb_children - 1]->type = NODE_ELSE;
    root->children[root->nb_children - 1]->data.ast_else = new_else;
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
    if (test == -1 || get_then(lex, new_elif->then)) //if one of them is an error, then 1
        return 1;
    
    int size = sizeof(struct ast) * (root->nb_children + 1);
    root->children = realloc(root->children, size);
    new_elif->count_cond = test;
    root->children[root->nb_children] = malloc(sizeof(struct ast));
    root->children[root->nb_children]->type = NODE_ELIF;
    root->children[root->nb_children]->data.ast_elif = new_elif;
    root->children++;
    return 0;
}

//builds and return ast_if_root if command is if
struct ast_if_root *build_ast_if(struct lexer *lex)
{
    //creating the ast_if_root node
    struct ast_if_root *new_root = create_if_root();
    new_root->children = malloc(sizeof(struct ast_if));
    //here getting if out of the lexer
    struct token *tip = lexer_pop(lex);
    //add if
    print(lex);
    if (build_if(lex, new_root))
        errx(2, "bad args in a wrong place in IF");
    printf("in the if\n");
    printf("%d\n", !lex);
    print(lex);
    while (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    if (lex && lexer_peek(lex)->type != TOKEN_ELSE && lexer_peek(lex)->type != TOKEN_ELIF && lexer_peek(lex)->type != TOKEN_FI)
        errx(2, "bad args in a wrong place after if");
    //add elif childs
    if (lexer_peek(lex)->type == TOKEN_FI)
        return new_root;
    while ((tip = lexer_pop(lex))->type != TOKEN_EOF && tip->type == TOKEN_ELIF)
    {
        if (build_elif(lex, new_root))
            errx(2, "bad args in a wrong place in ELIF");
        while (!lex || lexer_peek(lex)->type == TOKEN_EOF)
            lex = ask_entry();
    }
    printf("TOKEN TYPE \n%d\n",lexer_peek(lex)->type );
     printf("okkkkk\n");
    if (lexer_peek(lex)->type != TOKEN_ELSE && lexer_peek(lex)->type != TOKEN_FI)
        errx(2, "bad args in a wrong place after elif");
    //add else childs if existing
    if (lexer_peek(lex)->type == TOKEN_ELSE)
        build_else(lex, new_root);
    while (!lex || lexer_peek(lex)->type == TOKEN_EOF)
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
struct ast *build_ast(struct lexer *lex)
{
    struct ast *new_ast = malloc(sizeof(struct ast));
    new_ast->type = NODE_ROOT;
    struct ast_main_root *ast = create_main_root();
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    int count = 30;
    ast->children = malloc(sizeof(struct ast *) * 30);
    while (lex && lexer_peek(lex)->type != TOKEN_EOF)
    {
        ast->nb_children++;
        if (ast->nb_children >= count)
        {
            count *= 2;
            ast->children = realloc(ast->children, sizeof(struct ast *) * count);
        }
        if (lexer_peek(lex)->type == TOKEN_IF)
        {
            ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
            ast->children[ast->nb_children - 1]->type = NODE_IF_ROOT;
            ast->children[ast->nb_children - 1]->data.ast_if_root = build_ast_if(lex);
            printf("\n\n\nTYPE OF DATA IF = %d\n\n\n", ast->children[ast->nb_children - 1]->type);
            printf("if finished\n");
            print(lex);
        }
        if (lexer_peek(lex)->type == TOKEN_WORDS)
        {
            printf("then\n");
            struct ast_command **new_com = create_command();
            int test = get_command(lex, new_com);
            if (test == -1)
                errx(2, "couldn't get condition");
            for (int ind = 0; ind < test; ind++)
            {
                ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
                if (ast->nb_children >= count)
                {
                    count *= 2;
                    ast->children = realloc(ast->children, sizeof(struct ast *) * count);
                }
                printf("TESTHOLULULULULULU\n\n\n");
                ast->children[ast->nb_children - 1]->type = NODE_COMMAND;
                printf("TESTHOLALLALALALA\n\n\n");
                ast->children[ast->nb_children - 1]->data.ast_command = new_com[ind];
                ast->nb_children++;
            }
            ast->nb_children--;
        }
        if (lexer_peek(lex)->type == TOKEN_FI)
            break;
        if (lexer_peek(lex)->type == TOKEN_ELIF)
            break;
        if (lexer_peek(lex)->type == TOKEN_THEN)
            break;
        if (lexer_peek(lex)->type == TOKEN_ELSE)
            break;
    }
    new_ast->data.ast_main_root = ast;
    printf("\n\n\nTYPE OF DATA = %d\n\n\n", new_ast->type);
    return new_ast;
}
