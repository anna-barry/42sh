#pragma once

#include "token.h"

/**
 * \page Lexer
 *
 * The lexer cuts some input text into block called tokens.

 * This process is done **on demand**: the lexer doesn't read the
 * input more than it needs, and only creates tokens when lexer_peek
 * or lexer_pop is called, and no token is available.
 *
 * "2 + 3" will produce 3 tokens:
 *   - TOKEN_NUMBER { .value = 2 }
 *   - TOKEN_PLUS
 *   - TOKEN_NUMBER { .value = 3 }
 */

struct lexer
{
    const char *input; ///< The input data
    size_t pos; ///< The current offset inside the input data
    struct token *current_tok; ///< The next token, if processed
};

struct info_lexer
{
    struct lexer *lexer; ///< The lexer
    size_t *cap; ///< Capacity of lexer
    size_t *index; ///< Number of tokens (always min 1 for EOF)
};

struct read_for_int
{
    size_t step; ///< Step
    size_t start; ///< Start
    size_t end; ///< End
};

struct cap_and_i
{
    size_t *i;
    size_t *cap;
    int mode; // 1 if word 0 if not
};

/**
 * \brief Creates a new lexer
 */
struct info_lexer *lexer_init();

/**
 * \brief Adds in lexer given an input string.
 */
void lexer_new(const char *input, struct info_lexer *info);

/**
 ** \brief Free the given lexer, but not its input.
 */
void clear_info(struct info_lexer *new);
void lexer_info_free(struct info_lexer *info);
void lexer_free(struct lexer *lexer);

/**
 * \brief Returns the next token, but doesn't move forward: calling lexer_peek
 * multiple times in a row always returns the same result. This functions is
 * meant to help the parser check if the next token matches some rule.
 */
struct token *lexer_peek(struct lexer *lexer);

/**
 * \brief Returns the next token, and removes it from the stream:
 *   calling lexer_pop in a loop will iterate over all tokens until EOF.
 */
struct token *lexer_pop(struct lexer *lexer);

/**
 * \brief Get condition for for loop
 */
void get_for(const char *input, struct cap_and_i *i, size_t *index,
             struct lexer *new);

int is_end(char c);

struct cap_and_i *spe_token(const char *input, size_t *index,
                            struct cap_and_i *for_s, struct lexer *new);

/**
 * \brief Get condition Structure for for loop for execution purposes
 */
struct read_for_int *get_structure(char *input);
