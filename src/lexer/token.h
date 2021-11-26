#pragma once

#include <unistd.h>

enum token_type
{
    TOKEN_IF, ///< 'if'
    TOKEN_THEN, ///< 'then'
    TOKEN_ELIF, ///< 'elif'
    TOKEN_ELSE, ///< 'else'
    TOKEN_FI, ///< "fi"
    TOKEN_SEMICOLON, ///< ';'
    TOKEN_LINE_BREAK, ///< '\n'
    TOKEN_SIMPLE_QUOTE, ///< '''
    TOKEN_WORDS, ///< 'WORD+'
    TOKEN_EOF, ///< 'EOF'
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
