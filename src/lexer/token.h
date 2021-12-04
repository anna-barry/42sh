#pragma once

#include <unistd.h>

enum token_type
{
    TOKEN_IF, ///< 'if' -> 0
    TOKEN_THEN, ///< 'then' -> 1
    TOKEN_ELIF, ///< 'elif' -> 2
    TOKEN_ELSE, ///< 'else' -> 3
    TOKEN_FI, ///< "fi" -> 4
    TOKEN_SEMICOLON, ///< ';' -> 5
    TOKEN_LINE_BREAK, ///< '\n' -> 6
    TOKEN_SIMPLE_QUOTE, ///< ''' -> 7
    TOKEN_WORDS, ///< 'WORD+' -> 8
    TOKEN_EOF, ///< 'EOF' -> 9
    TOKEN_PIPE, ///< '|' -> 10
    TOKEN_REDIR_SORTIE, ///< '>' -> 11
    TOKEN_REDIR_ENTREE, ///< '<' -> 12
    TOKEN_REDIR_DESCRIPEUR, ///< '>&' -> 13
    TOKEN_REDIR_FIN_FICHIER, ///< '>>' -> 14
    TOKEN_REDIR_INPUT_DESCRIPEUR, ///< '<&' -> 15
    TOKEN_REDIR_RW, ///< '<>' -> 16
    TOKEN_UNTIL, ///< 'Until' -> 17
    TOKEN_DONE, ///< 'Done' -> 18
    TOKEN_DO, ///< 'Do' -> 19
    TOKEN_WHILE, ///< 'While' -> 20
    TOKEN_NEG, ///< '!' -> 21
    TOKEN_DOUBLE_QUOTE, ///< '"' -> 22
    TOKEN_AND, ///< '&&' -> 23
    TOKEN_OR, ///< '||' -> 24
    TOKEN_FOR, ///< 'For' -> 25  carreful: next word is variable
    TOKEN_IN, ///< 'In' -> 26 carreful: next word is sequence (1..10..2)
    TOKEN_FOR_WORD, ///< 'WORD+' -> 28
    TOKEN_FOR_SINGLE_QUOTE, ///< ''WORD+'' -> 29
    TOKEN_FOR_DOUBLE_QUOTE, ///< '"WORD+"' -> 30
    TOKEN_FOR_INT, ///< 'WORD+' with for {1..10..2}-> 29
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
