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
    TOKEN_REDIR_INPUT_DESCRIPEUR, ///< '<&' -> 15
    TOKEN_REDIR_PIPE, ///< '>|' -> 16
    TOKEN_REDIR_RW, ///< '<>' -> 17
    TOKEN_UNTIL, ///< 'Until' -> 18
    TOKEN_DONE, ///< 'Done' -> 19
    TOKEN_DO, ///< 'Do' -> 20
    TOKEN_WHILE, ///< 'While' -> 21
    TOKEN_NEG, ///< '!' -> 22
    TOKEN_DOUBLE_QUOTE, ///< '"' -> 23
    TOKEN_AND, ///< '&&' -> 24
    TOKEN_OR, ///< '||' -> 25
    TOKEN_FOR, ///< 'For' -> 26  carreful: next word is variable
    TOKEN_IN, ///< 'In' -> 27 carreful: next word is sequence (1..10..2)
    TOKEN_FOR_WORD, ///< 'WORD+' -> 28
    TOKEN_FOR_SINGLE_QUOTE, ///< ''WORD+'' -> 29
    TOKEN_FOR_DOUBLE_QUOTE, ///< '"WORD+"' -> 30
    TOKEN_FOR_INT, ///< 'WORD+' with for {1..10..2}-> 31
    TOKEN_BRACKETS_1, ///< '{' -> 32
    TOKEN_BRACKETS_2, ///< '}' -> 33
    TOKEN_PARENTHESE_1, ///< '(' -> 34
    TOKEN_PARENTHESE_2, ///< ')' -> 35
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
