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
    new_if->cond = malloc(sizeof(struct ast));
    new_if->then = malloc(sizeof(struct ast));
    return new_if;
}

struct ast_elif *create_elif()
{
    struct ast_elif *new_elif = malloc(sizeof(struct ast_elif));
    new_elif->count_cond = 0;
    new_elif->cond = malloc(sizeof(struct ast));
    new_elif->then = malloc(sizeof(struct ast));
    return new_elif;
}

struct ast_command *create_command()
{
    struct ast_command *new_c = malloc(sizeof(struct ast_command) * 30);
    new_c->option = NONE;
    new_c->count = 0;
    return new_c;
}

struct ast_simple_quote *create_simple_quote()
{
    struct ast_simple_quote *new_c = malloc(sizeof(struct ast_simple_quote) * 30);
    return new_c;
}

struct ast_double_quote *create_double_quote()
{
    struct ast_double_quote *new_c = malloc(sizeof(struct ast_double_quote) * 30);
    return new_c;
}

struct ast_pipe *create_pipe()
{
  struct ast_pipe *new = malloc(sizeof(struct ast_pipe));
  //no needed caus when added it has already been allocated;
  //new->left = malloc(sizeof(struct ast));
  new->right = malloc(sizeof(struct ast));
  return new;
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
int get_option(struct lexer *lex, struct ast_command *new)
{
  enum token_type type = lexer_peek(lex)->type;
    if (!lex || type == TOKEN_EOF)
      return 0;
    if (type == TOKEN_REDIR_ENTREE && type >= TOKEN_REDIR_FIN_FICHIER)
        new->option = REDIR_ENTREE;
    else if (type == TOKEN_REDIR_DESCRIPEUR)
        new->option = REDIR_DESCRIPEUR;
    else if (type == TOKEN_REDIR_SORTIE)
        new->option = REDIR_SORTIE;
    else if (type == TOKEN_REDIR_FIN_FICHIER)
        new->option = REDIR_FIN_FICHIER;
    else
        return 1;
    new->redir = strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
    return 0;
}
// build the command with conditions and handles error
//returns the number of command generated or -1 if error
int get_command(struct lexer *lex, struct ast_command *new)
{
  enum token_type type = lexer_peek(lex)->type;
    if (!lex || type == TOKEN_EOF)
        lex = ask_entry();
    if (type == TOKEN_SEMICOLON || type == TOKEN_LINE_BREAK)
    {
      new->argv = malloc(sizeof(char *));
      new->argv[0] = ";";
      new->count = 1;
      lexer_pop(lex);
      return 0;
    }
    if (type != TOKEN_WORDS)
        return 1;
    int capy = 30;
    new->argv = malloc(sizeof(char *) * 30);
    int y = 0;
    for (;lex && (type == TOKEN_WORDS); y++)
    {
        if (y >= capy)
        {
            capy *= 2;
            new->argv = realloc(new->argv, capy);
        }
        new->argv[y] = strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
            lexer_pop(lex);
            type = lexer_peek(lex)->type;
    }
    get_option(lex, new);
    new->count = y;
      type = lexer_peek(lex)->type;
      if (type == TOKEN_SEMICOLON || type == TOKEN_LINE_BREAK)
      {
        if (y >= capy)
        {
            capy *= 2;
            new->argv = realloc(new->argv, capy);
        }
        new->argv[y] = ";";
        new->count++;
        lexer_pop(lex);
      }
    return 0;
}
struct ast_pipe *get_pipe(struct ast *ast, struct lexer *lex);
// create the new node and calls build_ast for the struct
int get_then(struct lexer *lex, struct ast *new, enum ast_type mode)
{
    new->type = NODE_ROOT;
    if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        lex = ask_entry();
    new->data.ast_main_root = build_ast(lex, mode)->data.ast_main_root;
    //printf("\n\n\n\nGET COND : %d\n\n\n",new->data.ast_main_root->children[0]->type);
    if (mode == TOKEN_THEN)
    {
      if (lexer_peek(lex)->type == TOKEN_PIPE)
      {
        new->type = NODE_PIPE;
        new->data.ast_pipe = get_pipe(new,lex);
      }
    }
    return 0;
}

struct ast_pipe *get_pipe(struct ast *ast, struct lexer *lex)
{
  struct ast_pipe *new_pipe = create_pipe();
  new_pipe->left = ast;
  lexer_pop(lex);
  if (get_then(lex, new_pipe->right, NODE_THEN))
    errx(2, "wrong pipe implementation");
  return new_pipe;
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
    enum token_type type = lexer_peek(lex)->type;
    // here getting if out of the lexer
    lexer_pop(lex);
    // add if
    if (build_if(lex, new_root))
        errx(2, "bad args in a wrong place in IF");
    while (!lex || type == TOKEN_EOF)
        lex = ask_entry();
    // checking for errors
    type = lexer_peek(lex)->type;
    if (lex && type != TOKEN_ELSE && type != TOKEN_ELIF && type != TOKEN_FI)
        errx(2, "bad args in a wrong place after if");

    // if/while elif childs are existing adding them
    while (type != TOKEN_EOF && type == TOKEN_ELIF)
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
        while (!lex || type == TOKEN_EOF)
            lex = ask_entry();
        type = lexer_peek(lex)->type;
    }
    // checking for errors
    if (type != TOKEN_ELSE && type != TOKEN_FI)
        errx(2, "bad args in a wrong place after elif");

    // add else childs if existing
    if (type == TOKEN_ELSE)
    {
        if (new_root->nb_children >= cap)
        {
            cap *= 2;
            new_root->children =
                realloc(new_root->children, sizeof(struct ast_if) * cap);
        }
        build_else(lex, new_root);
    }
    type = lexer_peek(lex)->type;
    while (!lex || type == TOKEN_EOF)
        lex = ask_entry();
    // checking TOKEN FI ends the command
    type = lexer_peek(lex)->type;
    if (type != TOKEN_FI)
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
}

// PROCESS AND ADD CHILD WHEN COMMAND
void make_command(struct ast_main_root *ast, struct lexer *lex)
{
    struct ast_command *new_com = create_command();
    if (get_command(lex, new_com))
        errx(2, "couldn't get condition");
    int rank = ast->nb_children - 1;
    ast->children[rank] = malloc(sizeof(struct ast));
    //to be tested
      ast->children[rank]->type = NODE_COMMAND;
      ast->children[rank]->data.ast_command = new_com;
}

//make single quote
void make_simple_quote(struct ast_main_root *ast, struct lexer *lex)
{
    int rank = ast->nb_children - 1;
    ast->children[rank] = malloc(sizeof(struct ast));
    ast->children[rank]->type = NODE_SIMPLE_QUOTE;
    ast->children[rank]->data.ast_simple_quote = malloc(sizeof(struct ast_simple_quote));
    ast->children[rank]->data.ast_simple_quote->argv =
    strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
    lexer_pop(lex);
}

void make_double_quote(struct ast_main_root *ast, struct lexer *lex)
{
    struct ast_double_quote *new_quote= create_double_quote();
    int rank = ast->nb_children - 1;
    ast->children[rank] = malloc(sizeof(struct ast));
    ast->children[rank]->type = NODE_DOUBLE_QUOTE;
    ast->children[rank]->data.ast_simple_quote = malloc(sizeof(struct ast_double_quote));
    ast->children[rank]->data.ast_double_quote->argv =
    strndup(lexer_peek(lex)->value, strlen(lexer_peek(lex)->value) + 1);
    lexer_pop(lex);
}

void make_neg(struct ast_main_root *ast, struct lexer *lex)
{
  int rank = ast->nb_children - 1;
  ast->children[rank] = malloc(sizeof(struct ast));
  ast->children[rank] = build_ast(lex, NODE_NEG);
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
        if (type == TOKEN_ELSE || type == TOKEN_ELIF || type == TOKEN_FI || type == TOKEN_PIPE)
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
    struct ast_main_root *ast = create_main_root();
    enum token_type type = lexer_peek(lex)->type;
    if (!lex || type == TOKEN_EOF)
        lex = ask_entry();
    int count = 30;
    ast->children = malloc(sizeof(struct ast *) * 30);
    type = type;
    while (lex && check_break(mode, type))
    {

      printf("TYPE = %d\n", type);
      print(lex);
        ast->nb_children++;
        if (ast->nb_children >= count)
        {
            count *= 2;
            ast->children =
                realloc(ast->children, sizeof(struct ast *) * count);
        }
        // IF WORD IS IF THEN MAKE IF
        if (type == TOKEN_IF)
            make_if(ast, lex);
        // IF WORD IS WORD OR SEMICOLON MAKE COMMAND
        else if (type == TOKEN_WORDS || type == TOKEN_SEMICOLON || type == TOKEN_LINE_BREAK)
            make_command(ast, lex);
        else if (type == TOKEN_SIMPLE_QUOTE)
            make_simple_quote(ast, lex);
        else if (type == TOKEN_DOUBLE_QUOTE)
            make_double_quote(ast, lex);
        else if (type == TOKEN_NEG)
        {
            make_neg(ast, lex);
            //negation must break when the command was treated
            break;
        }
        else
            errx(2, "wrong implementation");
        type = lexer_peek(lex)->type;
        printf("TYPE = %d\n", type);
        print(lex);
    }
    struct ast *new_ast = malloc(sizeof(struct ast));
    new_ast->data.ast_main_root = ast;
    new_ast->type = NODE_ROOT;
    return new_ast;
}
