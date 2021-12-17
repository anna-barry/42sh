#include "parser.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//######################
// TO DELETE
//######################
void print(struct lexer *lexer)
{
    printf("START OF PRINTING ##########\n");
    char *tab[] = {
        [TOKEN_IF] = "If",         [TOKEN_THEN] = "Then",
        [TOKEN_ELIF] = "Elif",     [TOKEN_ELSE] = "Else",
        [TOKEN_FI] = "Fi",         [TOKEN_SEMICOLON] = ";",
        [TOKEN_LINE_BREAK] = "\n", [TOKEN_PIPE] = "|",
        [TOKEN_AND] = "&&",        [TOKEN_OR] = "||",
        [TOKEN_WHILE] = "While",   [TOKEN_DO] = "Do",
        [TOKEN_DONE] = "Done",     [TOKEN_UNTIL] = "Until",
        [TOKEN_FOR] = "For", [TOKEN_IN] = "In",
        [TOKEN_REDIR_ENTREE] = "<", [TOKEN_REDIR_SORTIE] = ">",
        [TOKEN_REDIR_DESCRIPEUR] = ">&",
        [TOKEN_REDIR_FIN_FICHIER] = ">>",
        [TOKEN_REDIR_INPUT_DESCRIPEUR] = "<&", ///< '<&' -> 15
        [TOKEN_REDIR_PIPE] = ">|",///< '>|' -> 16
        [TOKEN_REDIR_RW] = "<>", ///< '<>' -> 17
    };
    size_t i = 0;
    if (!lexer[0].current_tok || lexer[i].current_tok == NULL)
    {
      printf("END\n");
        return;
    }
    while (lexer[i].current_tok->type != TOKEN_EOF)
    {
        if (lexer[i].current_tok->type == TOKEN_SIMPLE_QUOTE || lexer[i].current_tok->type == TOKEN_FOR_SINGLE_QUOTE)
            printf("'%s'\n", lexer[i].current_tok->value);
        else if (lexer[i].current_tok->type == TOKEN_FOR_INT)
            printf("FOR INT\n");
        else if (lexer[i].current_tok->type == TOKEN_WORDS || lexer[i].current_tok->type == TOKEN_FOR_WORD)
            printf("%s\n", lexer[i].current_tok->value);
        else if (lexer[i].current_tok->type == TOKEN_DOUBLE_QUOTE || lexer[i].current_tok->type == TOKEN_FOR_DOUBLE_QUOTE)
            printf("\"%s\"\n", lexer[i].current_tok->value);
        else
            printf("%s\n", tab[lexer[i].current_tok->type]);
        i++;
    }
    printf("END OF PRINTING ##########\n");
}

//####################################################################
// BUILDING EACH STRUCTURE HANDLING ERRORS
//####################################################################
// returns 0 if ok, if error 1
// builds the if ast with cond ast and then ast
int build_if(struct info_lexer *i_lex, struct ast_if_root *root)
{
    struct ast_if *new_if = malloc(sizeof(struct ast_if));
    struct lexer *lex = i_lex->lexer;
    if (lexer_peek(lex)->type == TOKEN_THEN || (new_if->cond = get_then(i_lex, NODE_THEN)) == NULL)
        return 1;
    //printf("test\n");
    if (lexer_peek(lex)->type != TOKEN_EOF)
        token_free(lexer_pop(lex));
    // if one of them is an error, then 1
    enum token_type type = lexer_peek(lex)->type ;
    if (type == TOKEN_ELSE || type == TOKEN_ELIF || type == TOKEN_FI || (new_if->then = get_then(i_lex, NODE_IF)) == NULL)
        return 1;
    root->nb_children = 1;
    root->children[0] = malloc(sizeof(struct ast));
    root->children[0]->type = NODE_IF;
    root->children[0]->data.ast_if = new_if;
    return 0;
}

int build_else(struct info_lexer *i_lex, struct ast_if_root *root)
{
    struct ast_else *new_else = malloc(sizeof(struct ast_else));
    struct lexer *lex = i_lex->lexer;
    //poping the else
    token_free(lexer_pop(lex));
    if (lexer_peek(lex)->type == TOKEN_FI || (new_else->then = get_then(i_lex, NODE_ELSE)) == NULL)
        return 1;
    root->children[root->nb_children] = malloc(sizeof(struct ast));
    root->children[root->nb_children]->type = NODE_ELSE;
    root->children[root->nb_children]->data.ast_else = new_else;
    root->nb_children++;
    return 0;
}

int build_elif(struct info_lexer *i_lex, struct ast_if_root *root)
{
    struct ast_elif *new_elif = malloc(sizeof(struct ast_elif));
    struct lexer *lex = i_lex->lexer;
    if ((new_elif->cond = get_then(i_lex, NODE_THEN)) == NULL)
        return 1;
    token_free(lexer_pop(lex));
    enum token_type type = lexer_peek(lex)->type ;
    if (type == TOKEN_ELSE || type == TOKEN_ELIF || type == TOKEN_FI || (new_elif->then = get_then(i_lex, NODE_ELIF)) == NULL) // if one of them is an error, then 1
        return 1;
    root->children[root->nb_children] = malloc(sizeof(struct ast));
    root->children[root->nb_children]->type = NODE_ELIF;
    root->children[root->nb_children]->data.ast_elif = new_elif;
    root->nb_children++;
    return 0;
}

// builds global ast_if_root
struct ast_if_root *build_ast_if(struct info_lexer *i_lex)
{
    // creating the ast_if_root node
    struct lexer *lex = i_lex->lexer;
    struct ast_if_root *new_root = create_if_root();
    int cap = 30;
    new_root->children = malloc(sizeof(struct ast_if *) * cap);
    enum token_type type = lexer_peek(lex)->type;
    // here getting if out of the lexer
    token_free(lexer_pop(lex));
    // add if
    if (build_if(i_lex, new_root))
        err(2, "bad args in a wrong place in IF");
    /*while (!lex || type == TOKEN_EOF)
        ask_entry(i_lex);*/
    // checking for errors
    type = lexer_peek(lex)->type;
    if (lex && type != TOKEN_ELSE && type != TOKEN_ELIF && type != TOKEN_FI)
        err(2, "bad args in a wrong place after if");

    // if/while elif childs are existing adding them
    while (type != TOKEN_EOF && type == TOKEN_ELIF)
    {
        if (new_root->nb_children >= cap)
        {
            cap *= 2;
            new_root->children =
                realloc(new_root->children, sizeof(struct ast_if) * cap);
        }
        token_free(lexer_pop(lex));
        if (build_elif(i_lex, new_root))
            err(2, "bad args in a wrong place in ELIF");
        /*while (!lex || type == TOKEN_EOF)
            ask_entry(i_lex);*/
        type = lexer_peek(lex)->type;
    }
    // checking for errors
    if (type != TOKEN_ELSE && type != TOKEN_FI)
        err(2, "bad args in a wrong place after elif");

    // add else childs if existing
    if (type == TOKEN_ELSE)
    {
        if (new_root->nb_children >= cap)
        {
            cap *= 2;
            new_root->children =
                realloc(new_root->children, sizeof(struct ast_if) * cap);
        }
        build_else(i_lex, new_root);
    }
    type = lexer_peek(lex)->type;
    /*while (!lex || type == TOKEN_EOF)
        ask_entry(i_lex);*/
    // checking TOKEN FI ends the command
    type = lexer_peek(lex)->type;
    if (type != TOKEN_FI)
        err(2, "needed FI to close de IF condition");
    // remove the FI token
    token_free(lexer_pop(lex));
    /*if (!lex)
        ask_entry(i_lex);*/
    type = lexer_peek(lex)->type;
    if (type == TOKEN_IF || type == TOKEN_ELIF || type == TOKEN_ELSE || type == TOKEN_WHILE || type == TOKEN_FOR)
        errx(2, "need to close after if");
    else if (type == TOKEN_SEMICOLON)
    {
        token_free(lexer_pop(lex));
        type = lexer_peek(lex)->type;
        if (type == TOKEN_PIPE)
            errx(2, "shouldn't have putted ; before");
    }
    return new_root;
}

struct ast_while *build_ast_while(struct info_lexer *i_lex, int until)
{
    struct ast_while *new_root = malloc(sizeof(struct ast_while));
    struct lexer *lex = i_lex->lexer;
    // here getting if out of the lexer
    if (until)
    {
        new_root->cond = malloc(sizeof(struct ast));
        new_root->cond->type = NODE_NEG;
        new_root->cond->data.ast_neg = malloc(sizeof(struct ast_neg));
        if ((new_root->cond->data.ast_neg->node = get_then(i_lex, NODE_WHILE)) == NULL)
            err(2, "couldn't get cond in until");
    }
    else if((new_root->cond =get_then(i_lex, NODE_WHILE)) == NULL)
        err(2, "couldn't get cond in while");

    token_free(lexer_pop(lex));

    /*if (lexer_peek(lex)->type == TOKEN_EOF)
        ask_entry(i_lex);*/
    if ( (new_root->then = get_then(i_lex, NODE_DO)) == NULL)
        err(2, "couldn't get commands in while");
    /*if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
        ask_entry(i_lex);*/
    if (lexer_peek(lex)->type != TOKEN_DONE)
        err(2, "couldn't close while condition");
    token_free(lexer_pop(lex));
    /*if (!lex)
        ask_entry(i_lex);*/
    if (lexer_peek(lex)->type == TOKEN_SEMICOLON)
      token_free(lexer_pop(lex));
    else if (lexer_peek(lex)->type != TOKEN_DO && lexer_peek(lex)->type != TOKEN_EOF)
        errx(2, "need to close after while");
    return new_root;
}

struct ast_for *build_ast_for(struct info_lexer *i_lex)
{
  struct ast_for *new_for = malloc(sizeof(struct ast_for));
  struct lexer *lex = i_lex->lexer;
  token_free(lexer_pop(lex));
  if (lexer_peek(lex)->type != TOKEN_WORDS)
    err(2, "wrong implementation of variable : for <I> in");
  char *trans = lexer_peek(lex)->value;
  new_for->var = strndup(trans, strlen(trans) + 1);
  token_free(lexer_pop(lex));
  if (lexer_peek(lex)->type != TOKEN_IN)
    err(2, "wrong implementation of variable : for i <in>");
  token_free(lexer_pop(lex));
  enum token_type type = lexer_peek(lex)->type;
  if (type == TOKEN_FOR_INT)
{
    struct read_for_int* trans = get_structure(lexer_peek(lex)->value);
    new_for->cond = malloc(sizeof(struct ast));
    new_for->cond->type = NODE_FOR_INT;
    new_for->cond->data.ast_for_int = trans;
    token_free(lexer_pop(lex));
    if (lexer_peek(lex)->type != TOKEN_SEMICOLON)
        err(2, "missing a semicolone between for i in {...} [HERE] do ..;");
    token_free(lexer_pop(lex));

}
else if (lexer_peek(lex)->type != TOKEN_SEMICOLON)
    new_for->cond = get_then(i_lex, NODE_FOR_CHAR);
else
{
    new_for->cond = NULL;
    token_free(lexer_pop(lex));
}
  if (lexer_peek(lex)->type != TOKEN_DO)
    err(2, "missing th do instruction :for i in {...}; [DO] .. [DONE];");
  token_free(lexer_pop(lex));
  //now get the get then with the commands to execute
  if ((new_for->then = get_then(i_lex, NODE_DO)) == NULL)
      err(2, "couldn't get commands in while");
  /*if (!lex || lexer_peek(lex)->type == TOKEN_EOF)
          ask_entry(i_lex);*/
  if (lexer_peek(lex)->type != TOKEN_DONE)
      err(2, "wrong implementation in for: for i in x do commands <done>");
  token_free(lexer_pop(lex));

//printf("token type is = %d\n == %d ?",lexer_peek(lex)->type, TOKEN_SEMICOLON );
    if (lexer_peek(lex)->type != TOKEN_SEMICOLON && lexer_peek(lex)->type != TOKEN_DO && lexer_peek(lex)->type != TOKEN_EOF)
      errx(2, "need to close after do");
    if (lexer_peek(lex)->type == TOKEN_SEMICOLON)
      token_free(lexer_pop(lex));
    //print(lex);
  return new_for;
}

//################################################################
//#####EACH FUNCTION TOOL IS HANDLED HERE TO ADD IN THE TREE######



//################################################################
//#######MAIN ROOT STRUCTURE HANDLING FUNCTIONS###################

// DEPEDENING ON THE ROOT OF THE CALL TO BUILD_AST THE BREAK CASE IS DIFFERENT
int check_break(enum ast_type mode, enum token_type type, int open )
{
    // ajouter gestion d'erreur ici avec les ; et les double pipe etc
    //printf("MODE = %d\n, TYPE = %d\n", mode, type);
    if ((mode == NODE_ROOT || open) && type == TOKEN_EOF)// each function must handle asking tnew info
        return 0;
    if (mode == NODE_PIPE && (type == TOKEN_SEMICOLON || type == TOKEN_EOF || type == TOKEN_LINE_BREAK || type == TOKEN_PIPE))
        return 0;
    if (open)
        return 1;
    if (mode == NODE_IF || mode == NODE_ELIF)
    {
        if (type == TOKEN_ELSE || type == TOKEN_ELIF || type == TOKEN_FI)
            return 0;
    }
    if ( mode == NODE_FOR_CHAR || mode == NODE_WHILE || mode == NODE_FOR_INT)
    {
      if (type == TOKEN_DO ) //|| type == TOKEN_SEMICOLON)
        return 0;
    }
    if (mode == NODE_DO && type == TOKEN_DONE)
        return 0;
    if (mode == NODE_THEN)
    {
        if (type == TOKEN_THEN || type == TOKEN_AND
            || type == TOKEN_OR)
            return 0;
    }
    if (mode == NODE_ELSE && type == TOKEN_FI)
        return 0;
    return 1;
}

// small begining of creation of global ast
// char is already lexed so need a function before to link everything
// for now only handles IF and commands (for now echo)
struct ast *build_ast(struct info_lexer *i_lex, enum ast_type mode)
{
    struct ast *new_ast = malloc(sizeof(struct ast));
    struct ast_main_root *ast = create_main_root();
    ast->children = malloc(sizeof(struct ast *) * 30);
    int count = 30;
    struct lexer *lex = i_lex->lexer;
    enum token_type type = lexer_peek(lex)->type;
    int open = 0;
    while (lex && check_break(mode, type, open))
    {
        //printf("MODE = %d\n", mode);
        //printf("TYPE = %d\n", type);
        print(lex);
        //ast->nb_children++;
        if (++ast->nb_children >= count)
        {
            count += 30;
            ast->children =
                realloc(ast->children, sizeof(struct ast *) * count);
        }
        if (!open && type == TOKEN_IF && mode != NODE_FOR_CHAR)
            make_if(ast, i_lex);
        else if (type == TOKEN_WORDS || type == TOKEN_FOR_WORD || type == TOKEN_SIMPLE_QUOTE || type == TOKEN_FOR_SINGLE_QUOTE || type == TOKEN_DOUBLE_QUOTE || type == TOKEN_FOR_DOUBLE_QUOTE)
        {
            open = 1;
            make_command(ast, i_lex);
        }
        else if (open && lexer_peek(lex)->type == TOKEN_AND)
        {
            get_and(ast->children[--ast->nb_children - 1], i_lex, mode);
            open = 0;
        }
        else if (open && lexer_peek(lex)->type == TOKEN_OR)
        {
            get_or(ast->children[--ast->nb_children - 1], i_lex, mode);
            open = 0;
        }
        else if ((open || mode == NODE_FOR_CHAR) && (type == TOKEN_SEMICOLON || type == TOKEN_LINE_BREAK))
        {
            open = 0;
            ast->nb_children--;
            token_free(lexer_pop(lex));
        }
        else if (type == TOKEN_LINE_BREAK)
        {
            ast->nb_children--;
            token_free(lexer_pop(lex));
        }
        else if (mode == NODE_FOR_CHAR) //if node for char the fllowing are not possible
            err(2, "wrong implementation in for");
        else if (type == TOKEN_NEG)
        {
            make_neg(ast, i_lex, mode);// negation must break when the command was treated
            break;
        }
        else if (type >= 11 && type <= 17)
            get_redir(ast, i_lex);
        else if (!open && type == TOKEN_WHILE)
            make_while(ast, i_lex, 0);
        else if (!open && type == TOKEN_UNTIL)
            make_while(ast, i_lex, 1);
        else if (!open && type == TOKEN_FOR)
            make_for(ast,i_lex);
        else if (type == TOKEN_PIPE)
            get_pipe(ast, i_lex);
        else
          err(2, "wrong implementation");
        if (lex && lexer_peek(lex))
          type = lexer_peek(lex)->type;
    }
    new_ast->data.ast_main_root = ast;
    new_ast->type = NODE_ROOT;
    return new_ast;
}
