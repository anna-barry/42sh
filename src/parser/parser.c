#include "parser.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//######################
//TO DELETE
//######################
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

//####################################################################
// INITIALIZER OF EVERY STRUCT
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

struct ast_command *create_command()
{
    struct ast_command *new_c = malloc(sizeof(struct ast_command) * 30);
    new_c->count = 0;
    return new_c;
}

struct ast_single_quote *create_single_quote()
{
    struct ast_single_quote *new_c = malloc(sizeof(struct ast_single_quote) * 30);
    return new_c;
}

struct ast_double_quote *create_double_quote()
{
    struct ast_double_quote *new_c = malloc(sizeof(struct ast_double_quote) * 30);
    return new_c;
}

struct ast_elif *create_elif()
{
    struct ast_elif *new_elif = malloc(sizeof(struct ast_elif));
    new_elif->count_cond = 0;
    new_elif->then = malloc(sizeof(struct ast));
    return new_elif;
}
// en gros juste ca demande des nouvelles données quoi
struct lexer *ask_entry(void)
{
    write(1, ">", 1);
    char buf[500]; // chiffre basic jsp mettre on testera
    scanf("%s", buf);
    return lexer_new(buf);
}

//####################################################################
// GET args
//####################################################################

// build the command with conditions and handles error
//returns the number of command generated or -1 if error
int get_command(struct lexer *lex, struct ast_command *new)
{
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    if (lexer_peek(lex)->type != TOKEN_WORDS)
        return 11;
    int capy = 30;
    new->argv = malloc(sizeof(char *) * 30);
    int y = 0;
    for (;lex && (lexer_peek(lex)->type == TOKEN_WORDS); y++)
    {
        if (y >= capy)
        {
            capy *= 2;
            new->argv = realloc(new->argv, capy);
        }
        new->argv[y] = strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
            lexer_pop(lex);
      }

      new->count = y;
      if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
         lex = ask_entry();
      if (lexer_peek(lex)->type == TOKEN_SEMICOLON)
        lexer_pop(lex);
    return 0;
}

// create the new node and calls build_ast for the struct
int get_then(struct lexer *lex, struct ast *new, enum ast_type mode)
{
    new = malloc(sizeof(struct ast));
    new->type = NODE_ROOT;
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    enum token_type trans = lexer_peek(lex)->type;
    new->data.ast_main_root = build_ast(lex, mode)->data.ast_main_root;
    return 0;
}

//####################################################################
// BUILDING EACH STRUCTURE HANDLING ERRORS
//####################################################################
// returns 0 if ok, if error 1
// builds the if ast with cond ast and then ast
int build_if(struct lexer *lex, struct ast_if_root *root)
{
    struct ast_if *new_if = create_if();

    if (get_then(lex, new_if->cond, NODE_THEN))
      return 1;

    lexer_pop(lex);
    // if one of them is an error, then 1
    if (get_then(lex, new_if->then, NODE_IF))
        return 1;
    root->nb_children = 1;
    root->children[0] = malloc(sizeof(struct ast));
    root->children[0]->type = NODE_IF;
    root->children[0]->data.ast_if = new_if;
    printf("IF BUILT\n");
    return 0;
}

int build_else(struct lexer *lex, struct ast_if_root *root)
{
    struct ast_else *new_else = create_else();
    lexer_pop(lex);
    if (get_then(lex, new_else->then, NODE_ELSE))
        return 1;
    // root->children = realloc(root->children, sizeof(struct ast) *
    // root->nb_children);
    root->children[root->nb_children] = malloc(sizeof(struct ast));
    root->children[root->nb_children]->type = NODE_ELSE;
    root->children[root->nb_children]->data.ast_else = new_else;
    root->nb_children++;
    return 0;
}

int build_elif(struct lexer *lex, struct ast_if_root *root)
{
    struct ast_elif *new_elif = create_elif();

    if (get_then(lex, new_elif->cond, NODE_THEN))
      return 1;

    lexer_pop(lex);

    if (get_then(lex, new_elif->then, NODE_ELIF)) // if one of them is an error, then 1
        return 1;

    int size = sizeof(struct ast) * (root->nb_children + 1);
    root->children[root->nb_children] = malloc(sizeof(struct ast));
    root->children[root->nb_children]->type = NODE_ELIF;
    root->children[root->nb_children]->data.ast_elif = new_elif;
    root->nb_children++;
    return 0;
}

// builds global ast_if_root
struct ast_if_root *build_ast_if(struct lexer *lex)
{
    // creating the ast_if_root node
    struct ast_if_root *new_root = create_if_root();
    int cap = 30;
    new_root->children = malloc(sizeof(struct ast_if) * cap);
    // here getting if out of the lexer
    lexer_pop(lex);
    // add if
    if (build_if(lex, new_root))
        errx(2, "bad args in a wrong place in IF");
    while (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
printf("okayyy\n");
    // checking for errors
    if (lex && lexer_peek(lex)->type != TOKEN_ELSE
        && lexer_peek(lex)->type != TOKEN_ELIF
        && lexer_peek(lex)->type != TOKEN_FI)
        errx(2, "bad args in a wrong place after if");

    // if/while elif childs are existing adding them
    while (lexer_peek(lex)->type != TOKEN_EOF
           && lexer_peek(lex)->type == TOKEN_ELIF)
    {
        if (new_root->nb_children >= cap)
        {
            cap *= 2;
            new_root->children =
                realloc(new_root->children, sizeof(struct ast_if) * cap);
        }
        lexer_pop(lex);
        if (build_elif(lex, new_root))
            errx(2, "bad args in a wrong place in ELIF");
        while (!lex || lexer_peek(lex)->type == TOKEN_EOF)
            lex = ask_entry();
    }
    // checking for errors
    if (lexer_peek(lex)->type != TOKEN_ELSE
        && lexer_peek(lex)->type != TOKEN_FI)
        errx(2, "bad args in a wrong place after elif");

    // add else childs if existing
    if (lexer_peek(lex)->type == TOKEN_ELSE)
    {
        if (new_root->nb_children >= cap)
        {
            cap *= 2;
            new_root->children =
                realloc(new_root->children, sizeof(struct ast_if) * cap);
        }
        build_else(lex, new_root);
    }

    while (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();

    // checking TOKEN FI ends the command
    if (lexer_peek(lex)->type != TOKEN_FI)
        errx(2, "needed FI to close de IF condition");
    // remove the FI token
    lexer_pop(lex);
    return new_root;
}

//################################################################
//#####EACH FUNCTION TOOL IS HANDLED HERE TO ADD IN THE TREE######

// PROCESS AND ADD CHILD WHEN IF
void make_if(struct ast_main_root *ast, struct lexer *lex)
{
    ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
    ast->children[ast->nb_children - 1]->type = NODE_IF_ROOT;
    ast->children[ast->nb_children - 1]->data.ast_if_root = build_ast_if(lex);
    printf("okayyyIF ENDED\n");
}

// PROCESS AND ADD CHILD WHEN COMMAND
void make_command(struct ast_main_root *ast, struct lexer *lex)
{
    struct ast_command *new_com = create_command();
    if (get_command(lex, new_com))
        errx(2, "couldn't get condition");

    ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
    ast->children[ast->nb_children - 1]->type = NODE_COMMAND;
    ast->children[ast->nb_children - 1]->data.ast_command = new_com;
}

//make single quote
void make_simple_quote(struct ast_main_root *ast, struct lexer *lex)
{
    ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
    ast->children[ast->nb_children - 1]->type = NODE_SINGLE_QUOTE;
    ast->children[ast->nb_children - 1]->data.ast_single_quote = malloc(sizeof(struct ast_single_quote));
    ast->children[ast->nb_children - 1]->data.ast_single_quote->argv =
    strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
    lexer_pop(lex);
}

void make_double_quote(struct ast_main_root *ast, struct lexer *lex)
{
    struct ast_double_quote *new_quote= create_double_quote();
    ast->children[ast->nb_children - 1] = malloc(sizeof(struct ast));
    ast->children[ast->nb_children - 1]->type = NODE_DOUBLE_QUOTE;
    ast->children[ast->nb_children - 1]->data.ast_single_quote = malloc(sizeof(struct ast_double_quote));
    ast->children[ast->nb_children - 1]->data.ast_double_quote->argv =
    strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
    lexer_pop(lex);
}


//################################################################
//#######MAIN ROOT STRUCTURE HANDLING FUNCTIONS###################

// DEPEDENING ON THE ROOT OF THE CALL TO BUILD_AST THE BREAK CASE IS DIFFERENT
int check_break(enum ast_type mode, enum token_type type)
{
    if (mode == NODE_ROOT && type == TOKEN_EOF)
        return 0;
    if (mode == NODE_ROOT && (type == TOKEN_ELSE || type == TOKEN_ELIF))
        errx(2, "wrong implementation in node root");
    if (mode == NODE_IF || mode == NODE_ELIF)
    {
        if (type == TOKEN_ELSE || type == TOKEN_ELIF || type == TOKEN_FI)
            return 0;
    }
    if (mode == NODE_THEN && type == TOKEN_THEN)
      return 0;
    if (mode == NODE_ELSE && type == TOKEN_FI)
        return 0;
    return 1;
}

// small begining of creation of global ast
// char is already lexed so need a function before to link everything
// for now only handles IF and commands (for now echo)
struct ast *build_ast(struct lexer *lex, enum ast_type mode)
{
    struct ast *new_ast = malloc(sizeof(struct ast));
    struct ast_main_root *ast = create_main_root();
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    int count = 30;
    ast->children = malloc(sizeof(struct ast *) * 30);
    while (lex && check_break(mode, lexer_peek(lex)->type))
    {
      printf("TYPE = %d\n", lexer_peek(lex)->type);
      print(lex);
        ast->nb_children++;
        if (ast->nb_children >= count)
        {
            count *= 2;
            ast->children =
                realloc(ast->children, sizeof(struct ast *) * count);
        }
        // IF WORD IS IF THEN MAKE IF
        if (lexer_peek(lex)->type == TOKEN_IF)
            make_if(ast, lex);
        // IF WORD IS WORD OR SEMICOLON MAKE COMMAND
        else if (lexer_peek(lex)->type == TOKEN_WORDS)
            make_command(ast, lex);
        else if (lexer_peek(lex)->type == TOKEN_SIMPLE_QUOTE)
            make_simple_quote(ast, lex);
        /*else if (lexer_peek(lex)->type == TOKEN_DOUBLE_QUOTE)
            make_double_quote(ast, lex);*/
        else
            errx(2, "wrong implementation");
    }
    new_ast->data.ast_main_root = ast;
    new_ast->type = NODE_ROOT;
    return new_ast;
}
