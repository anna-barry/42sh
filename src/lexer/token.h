#pragma once

#include <unistd.h>

enum token_type
{
    TOKEN_IF, ///< 'if' -> 1
    TOKEN_THEN, ///< 'then' -> 2
    TOKEN_ELIF, ///< 'elif' -> 3
    TOKEN_ELSE, ///< 'else' -> 4
    TOKEN_FI, ///< "fi" -> 5
    TOKEN_SEMICOLON, ///< ';' -> 6
    TOKEN_LINE_BREAK, ///< '\n' -> 7
    TOKEN_SIMPLE_QUOTE, ///< ''' -> 8
    TOKEN_WORDS, ///< 'WORD+' -> 9
    TOKEN_EOF, ///< 'EOF' -> 10
    TOKEN_PIPE, ///< '|' -> 10
    TOKEN_REDIR_SORTIE, ///< '>' -> 11
    TOKEN_REDIR_ENTREE, ///< '<' -> 12
    TOKEN_REDIR_DESCRIPEUR, ///< '>&' -> 13
    TOKEN_REDIR_FIN_FICHIER, ///< '>>' -> 14
    TOKEN_UNTIL, ///< 'Until' -> 15
    TOKEN_DONE, ///< 'Done' -> 16
    TOKEN_DO, ///< 'Do' -> 17
    TOKEN_WHILE, ///< 'While' -> 18
    TOKEN_NEG, ///< '!' -> 19
    TOKEN_DOUBLE_QUOTE, ///< '"' -> 20
    TOKEN_AND, ///< '&&' -> 21
    TOKEN_OR, ///< '||' -> 22
    TOKEN_ERROR ///< tt's not a real token, it's returned in case of invalid
                ///< input
};

struct token
{
    enum token_type type; ///< The kind of token
    char *value; ///< If the token is a number, its value
};

/**
 * \brief Allocate a new token
 */
struct token *token_new(enum token_type type);

/**
 * \brief Frees a token
 */
void token_free(struct token *token);
