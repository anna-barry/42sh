#include "lexer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>


int is_space(char c)
{
  return c == '\t' || c == '\n' || c == ' ';
}

int is_end(char c)
{
  return c == ';' || c == '\n' || c == ' ' || c == '\t';
}

size_t spe_token(const char *input, size_t index, size_t i, struct lexer *new)
{
  size_t nb = 0;
  size_t len = strlen(input);
  char current_char = input[i];
  if (current_char >= 'A' && current_char <= 'Z')
    current_char = current_char + ('a' - 'A');
  if (current_char >= 'a' && current_char <= 'z')
  {
    /*
     * testing if
     */
      if (current_char == 'i' &&  i + 1 < len && input[i + 1] == 'f')
      {
        new[index - 1].current_tok = token_new(TOKEN_IF);
        new[index - 1].current_tok->value = NULL;
        nb = 1;
      }
      /*
       * testing else
       */
      else if (current_char == 'e' &&  i + 3 < len && input[i + 1] == 'l'
                && input[i + 2] == 's' && input[i + 3] == 'l')
      {
        new[index - 1].current_tok = token_new(TOKEN_ELSE);
        new[index - 1].current_tok->value = NULL;
        nb = 3;
      }
      /*
       * testing elif
       */
      else if (current_char == 'e' &&  i + 3 < len && input[i + 1] == 'l'
                && input[i + 2] == 'i' && input[i + 3] == 'f')
      {
        new[index - 1].current_tok = token_new(TOKEN_ELIF);
        new[index - 1].current_tok->value = NULL;
        nb = 3;
      }
      /*
       * testing then
       */
       else if (current_char == 't' &&  i + 3 < len && input[i + 1] == 'h'
                 && input[i + 2] == 'e' && input[i + 3] == 'n')
       {
         new[index - 1].current_tok = token_new(TOKEN_THEN);
         new[index - 1].current_tok->value = NULL;
         nb = 3;
       }
       /*
        * testing fi
        */
        else if (current_char == 'f' &&  i + 1 < len && input[i + 1] == 'i')
        {
          new[index - 1].current_tok = token_new(TOKEN_FI);
          new[index - 1].current_tok->value = NULL;
          nb = 1;
        }
        else
        {
          /*
           * testing word
           */
          new[index - 1].current_tok = token_new(TOKEN_WORDS);
          nb = 0;
          while (i + nb < strlen(input) && ((input[i + nb] >= 'a'
                && input[i + nb] <= 'z') ||
                (input[i + nb] >= 'A' && input[i + nb] <= 'Z')))
          {
              nb++;
          }
          new[index - 1].current_tok->value = strndup(input + i, nb);
          new[index - 1].current_tok->value[nb] = '\0';
        }
  }
  else
  {
    new[index - 1].current_tok = token_new(TOKEN_ERROR);
    fprintf(stderr, "error in token\n");
  }
  return nb;
}

struct lexer *lexer_new(const char *input)
{
    size_t cap = 20;
    struct lexer *new = malloc(20 * sizeof(struct lexer));
    size_t i = 0;
    size_t index = 1;
    size_t nb = 0;
    for (; i <= strlen(input); i++)
    {
      nb = 1;
      if (is_space(input[i]))
        continue;
      if (cap < (i + 1))
      {
          cap *= 2;
          new = realloc(new, cap * sizeof(struct lexer));
      }
      switch (input[i])
      {
          case ('\n'):
              new[index - 1].current_tok = token_new(TOKEN_LINE_BREAK);
              new[index - 1].current_tok->value = NULL;
              break;
          case ('\0'):
              new[index - 1].current_tok = token_new(TOKEN_EOF);
              new[index - 1].current_tok->value = NULL;
              break;
          case (';'):
              new[index - 1].current_tok = token_new(TOKEN_SEMICOLON);
              new[index - 1].current_tok->value = NULL;
              break;
          case ('\''):
              new[index - 1].current_tok = token_new(TOKEN_SIMPLE_QUOTE);
              nb = 1;
              while (i + nb < strlen(input) && input[i + nb] != '\0'
              && input[i + nb] != '\'')
              {
                nb++;
              }
              new[index - 1].current_tok->value = strndup(input + i, nb + 2);
              new[index - 1].current_tok->value[nb + 1] = '\0';
              break;
          default:
              nb = spe_token(input, index, i, new);
              break;
        }
        new[index].input = input;
        new[index].pos = i;
        i += nb;
        index++;
    }
    return new;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer)
      free(lexer);
    lexer = NULL;
}

struct token *lexer_peek(struct lexer *lexer)
{
    return lexer->current_tok;
}


struct token *lexer_pop(struct lexer *lexer)
{
    if (lexer == NULL)
        return NULL;
    if (lexer->current_tok->type == TOKEN_EOF)
    {
        return lexer->current_tok;
    }

    // 1) first must go to second
    // 2) lexer[0] should be returned
    // 3) all should be exchanged

    enum token_type t = lexer->current_tok->type;
    char *test = NULL;
    if (t == TOKEN_SIMPLE_QUOTE || t == TOKEN_WORDS)
    {
      test = strndup( lexer->current_tok->value,
        strlen(lexer->current_tok->value));
    }
    size_t j = 0;
    while (lexer[j].current_tok->type != TOKEN_EOF) {
      if (j > 0)
      {
        lexer[j].pos = lexer[j + 1].pos;
      }
      lexer[j].current_tok->type = lexer[j + 1].current_tok->type;
      if (lexer[j + 1].current_tok->type == TOKEN_SIMPLE_QUOTE
        || lexer[j + 1].current_tok->type == TOKEN_WORDS)
      {
        if (lexer[j].current_tok->value != NULL) {
          free(lexer[j].current_tok->value);
          lexer[j].current_tok->value = NULL;
        }
        lexer[j].current_tok->value = strndup(lexer[j + 1].current_tok->value,
           strlen(lexer[j + 1].current_tok->value));
      }
      j++;
    }
    if (t == TOKEN_SIMPLE_QUOTE || t == TOKEN_WORDS)
    {
      if (lexer[j].current_tok->value != NULL) {
        free(lexer[j].current_tok->value);
        lexer[j].current_tok->value = NULL;
      }
      lexer[j].current_tok->value = test;
    }
    lexer[j].current_tok->type = t;
    return lexer[j].current_tok;
}
