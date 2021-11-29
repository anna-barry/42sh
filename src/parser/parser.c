#include "parser.h"

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"

void print(struct lexer *lexer)
{
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
}

struct ast_main_root *build_ast(struct lexer *lex);
//creating and allocating space for node
//type is here to determine wich type of struct has to be created
union ast_data *create_node(enum ast_type type)
{
    union ast_data *new = malloc(sizeof(union ast_data));
    if (type == NODE_ROOT)
    {
        struct ast_main_root *ast = malloc(sizeof(struct ast_main_root));
        ast->nb_children = 0;
        ast->children = NULL;
        ast->type.type = NODE_ROOT;
        new->ast_main_root = ast;
    }
    if (type == NODE_IF_ROOT)
    {
        struct ast_if_root *root = malloc(sizeof(struct ast_if_root));
        root->type.type = NODE_IF_ROOT;
        root->nb_children = 0;
        root->status = 1;
        root->children = NULL;
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
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    if (lexer_peek(lex)->type != TOKEN_WORDS)
        return 1;
    int i = 0;
    new = malloc(sizeof(struct ast_command));
    new->type.type = NODE_COMMAND;
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
        if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
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
    if (!lex  || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    if (lexer_peek(lex)->type != TOKEN_THEN)
        return 1;
    print(lex);
    printf("END§§§§§§§§§§§§§§§§\n");
    lexer_pop(lex);
    print(lex);
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
    new_root->children = malloc(sizeof(struct ast_if));
    //here getting if out of the lexer
    struct token *tip = lexer_pop(lex);
    //add if
    print(lex);
    printf("before build if\n");
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
struct ast_main_root *build_ast(struct lexer *lex)
{
    printf("ici2\n");
    struct ast_main_root *ast = create_node(NODE_IF_ROOT)->ast_main_root;
    printf("ici2\n");
    ast->children = malloc(sizeof(union ast_data));
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    while (lex && lexer_peek(lex)->type != TOKEN_EOF)
    {
        printf("data = %s\n", lex->input);
        printf("pos = %zu\n", lex->pos);
        ast->nb_children++;
        ast->children = realloc(ast->children, sizeof(union ast_data) * ast->nb_children);
        if (lexer_peek(lex)->type == TOKEN_IF)
        {
            ast->children[ast->nb_children - 1].ast_if_root = build_ast_if(lex);
            printf("if finished\n");
        }
        if (lexer_peek(lex)->type == TOKEN_WORDS)
        {
            printf("then\n");
            struct ast_command *new_com = create_node(NODE_COMMAND)->ast_command;
            if (get_command(lex, new_com))
                errx(2, "couldn't get condition");
            ast->children[ast->nb_children - 1].ast_command = new_com;
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
    return ast;
}

int main()
{
    struct lexer *lexer = lexer_new("if echo b; then echo a; fi");
    print(lexer);
    struct ast_main_root *ast = build_ast(lexer);
    free(ast);
    return 0;
}
