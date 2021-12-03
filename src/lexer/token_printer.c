#include <stdio.h>

#include "lexer.h"

char *tab[] = {
    [TOKEN_IF] = "If", [TOKEN_THEN] = "Then",  [TOKEN_ELIF] = "Elif",
    [TOKEN_ELSE] = "Else",  [TOKEN_FI] = "Fi", [TOKEN_SEMICOLON] = ";",
    [TOKEN_LINE_BREAK] = "\n", [TOKEN_PIPE] = "|", [TOKEN_NEG] = "!", 
    [TOKEN_WHILE] = "While", [TOKEN_DO] = "Do", [TOKEN_DONE] = "Done",
    [TOKEN_UNTIL] = "Until", [TOKEN_AND] = "&&", [TOKEN_OR] = "||",
    [TOKEN_FOR] = "For", [TOKEN_IN] = "In", [TOKEN_REDIR_INPUT_DESCRIPEUR] = "<&",
    [TOKEN_REDIR_RW] = "<>",
};

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = lexer_new(argv[1]);
    struct token *token = lexer_pop(lexer);
    while (token->type != TOKEN_EOF && token->type != TOKEN_ERROR)
    {
        if (token->type == TOKEN_SIMPLE_QUOTE
        || token->type == TOKEN_WORDS 
        || token->type == TOKEN_REDIR_SORTIE 
        || token->type == TOKEN_REDIR_ENTREE 
        || token->type == TOKEN_REDIR_DESCRIPEUR
        || token->type == TOKEN_REDIR_FIN_FICHIER
        || token->type == TOKEN_DOUBLE_QUOTE
        || token->type == TOKEN_FOR_WORD
        || token->type == TOKEN_FOR_SINGLE_QUOTE
        || token->type == TOKEN_FOR_DOUBLE_QUOTE
        || token->type == TOKEN_FOR_INT)
            printf("%s\n", token->value);
        else
            printf("%s\n", tab[token->type]);

        token_free(token);
        token = lexer_pop(lexer);
    }

    if (token->type == TOKEN_EOF)
        printf("EOF\n");

    token_free(token);
    lexer_free(lexer);

    return 0;
}
