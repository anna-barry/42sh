#include <stdio.h>

#include "lexer.h"

char *tab[] = {
    [TOKEN_IF] = "If", [TOKEN_THEN] = "Then",  [TOKEN_ELIF] = "Elif",
    [TOKEN_ELSE] = "Else",  [TOKEN_FI] = "Fi", [TOKEN_SEMICOLON] = ";",
    [TOKEN_LINE_BREAK] = "\n"
};

int testing_token(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = lexer_new(argv[1]);
    struct token *token = lexer_pop(lexer);
    while (token->type != TOKEN_EOF && token->type != TOKEN_ERROR)
    {
        if (token->type == TOKEN_SIMPLE_QUOTE || token->type == TOKEN_WORDS)
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
