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
  return c == ';' || c == '\n' || c == ' ' || c == '\t' || c == '[' || c == ']';
}

void lexer_new(const char *input, struct info_lexer *info)
{
    size_t *i = malloc(sizeof(size_t));
    *i = 0;
    size_t nb = 0;
    int mode = 0;
    for (; *i <= strlen(input); *i = *i + 1)
    {
      nb = 0;
      if (is_space(input[*i]) || input[*i] == '[' || input[*i] == ']')
        continue;
      if (*info->cap < (*info->index + 2))
      {
          *info->cap *= 2;
          info->lexer = realloc(info->lexer, *info->cap * sizeof(struct lexer));
      }
      switch (input[*i])
      {
          case ('\n'):
              info->lexer[*info->index - 1].current_tok = token_new(TOKEN_LINE_BREAK);
              info->lexer[*info->index - 1].current_tok->value = NULL;
              if (mode == 1)
                mode = 0;
              break;
             /*
               * Testing for end of file
               */
          case ('\0'):
              info->lexer[*info->index - 1].current_tok = token_new(TOKEN_EOF);
              info->lexer[*info->index - 1].current_tok->value = NULL;
              if (mode == 1)
                mode = 0;
              //printf("end of file \n");
              break;
          case (';'):
              info->lexer[*info->index - 1].current_tok = token_new(TOKEN_SEMICOLON);
              info->lexer[*info->index - 1].current_tok->value = NULL;
              if (mode == 1)
                mode = 0;
              //printf("lol;;;;;lol\n");
              break;
              /*
               * Testing for pipeline
               */
          case ('|'):
              if (mode == 1)
                mode = 0;
              if (*i + 1 < strlen(input) && input[*i + 1] == '|')
              {
                info->lexer[*info->index - 1].current_tok = token_new(TOKEN_OR);
                info->lexer[*info->index - 1].current_tok->value = NULL;
                nb++;
              }
              else
              {
                info->lexer[*info->index - 1].current_tok = token_new(TOKEN_PIPE);
                info->lexer[*info->index - 1].current_tok->value = NULL;
              }
              break;
          case ('!'):
              info->lexer[*info->index - 1].current_tok = token_new(TOKEN_NEG);
              info->lexer[*info->index - 1].current_tok->value = NULL;
              break;
              /*
               * Testing for Redirections
               */
          case ('<'):
              if (mode == 1)
                mode = 0;
              if (*i + 1 < strlen(input) && input[*i + 1] == '&')
              {
                info->lexer[*info->index - 1].current_tok = token_new(TOKEN_REDIR_INPUT_DESCRIPEUR);
                nb = 2;
                while (input[*i + nb] == ' ')
                {
                  nb++;
                }
                if (input[*i + 2] >= '0' && input[*i + 2] <= '9')
                {
                  size_t tmp = nb;
                  while (*i + nb < strlen(input) && !is_end(input[*i + nb]))
                  {
                    nb++;
                  }
                  info->lexer[*info->index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                  info->lexer[*info->index - 1].current_tok->value[nb - tmp] = '\0';
                  nb--;
                }
                else
                {
                  char *tmp = malloc(2 * sizeof(char));
                  tmp[0] = '0';
                  tmp[1] = '\0';
                  info->lexer[*info->index - 1].current_tok->value = tmp;
                  nb--;
                }
              }
              else if (*i + 1 < strlen(input) && input[*i + 1] == '>')
              {
                info->lexer[*info->index - 1].current_tok = token_new(TOKEN_REDIR_RW);
                nb = 2;
                while (input[*i + nb] == ' ')
                {
                  nb++;
                }
                size_t tmp = nb;
                while (*i + nb < strlen(input) && !is_end(input[*i + nb]))
                {
                  nb++;
                }
                info->lexer[*info->index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                info->lexer[*info->index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              else
              {
                info->lexer[*info->index - 1].current_tok = token_new(TOKEN_REDIR_ENTREE);
                nb = 1;
                while (input[*i + nb] == ' ')
                {
                  nb++;
                }
                size_t tmp = nb;
                while (*i + nb < strlen(input) && !is_end(input[*i + nb]))
                {
                  nb++;
                }
                info->lexer[*info->index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                info->lexer[*info->index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              break;
              break;
          case ('>'):
              if (mode == 1)
                mode = 0;
              if (*i + 1 < strlen(input) && input[*i + 1] == '&')
              {
                info->lexer[*info->index - 1].current_tok = token_new(TOKEN_REDIR_DESCRIPEUR);
                nb = 2;
                while (input[*i + nb] == ' ')
                {
                  nb++;
                }
                if (input[*i + nb] >= '0' && input[*i + nb] <= '9')
                {
                  size_t tmp = nb;
                  while (*i + nb < strlen(input) && !is_end(input[*i + nb]))
                  {
                    nb++;
                  }
                  info->lexer[*info->index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                  info->lexer[*info->index - 1].current_tok->value[nb - tmp] = '\0';
                  nb--;
                }
                else
                {
                  char *tmp = malloc(2 * sizeof(char));
                  tmp[0] = '1';
                  tmp[1] = '\0';
                  info->lexer[*info->index - 1].current_tok->value = tmp;
                  nb--;
                }
              }
              else if (*i + 1 < strlen(input) && input[*i + 1] == '>')
              {
                info->lexer[*info->index - 1].current_tok = token_new(TOKEN_REDIR_FIN_FICHIER);
                nb = 2;
                while (input[*i + nb] == ' ')
                {
                  nb++;
                }
                size_t tmp = nb;
                while (*i + nb < strlen(input) && !is_end(input[*i + nb]))
                {
                  nb++;
                }
                info->lexer[*info->index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                info->lexer[*info->index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              else if (*i + 1 < strlen(input) && input[*i + 1] == '|')
              {
                info->lexer[*info->index - 1].current_tok = token_new(TOKEN_REDIR_PIPE);
                nb = 2;
                while (input[*i + nb] == ' ')
                {
                  nb++;
                }
                size_t tmp = nb;
                while (*i + nb < strlen(input) && !is_end(input[*i + nb]))
                {
                  nb++;
                }
                info->lexer[*info->index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                info->lexer[*info->index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              else
              {
                info->lexer[*info->index - 1].current_tok = token_new(TOKEN_REDIR_SORTIE);
                nb = 1;
                while (input[*i + nb] == ' ')
                {
                  nb++;
                }
                size_t tmp = nb;
                while (*i + nb < strlen(input) && !is_end(input[*i + nb]))
                {
                  nb++;
                }
                info->lexer[*info->index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                info->lexer[*info->index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              break;
          case ('\''):
              info->lexer[*info->index - 1].current_tok = token_new(TOKEN_SIMPLE_QUOTE);
              nb = 1;
              while (*i + nb < strlen(input) && input[*i + nb] != '\0'
              && input[*i + nb] != '\'')
              {
                nb++;
              }
              info->lexer[*info->index - 1].current_tok->value = strndup(input + *i + 1, nb - 1);
              info->lexer[*info->index - 1].current_tok->value[nb - 1] = '\0';
              break;
          case ('\"'):
              info->lexer[*info->index - 1].current_tok = token_new(TOKEN_DOUBLE_QUOTE);
              nb = 1;
              while (*i + nb < strlen(input) && input[*i + nb] != '\0'
              && input[*i + nb] != '\"')
              {
                nb++;
              }
              info->lexer[*info->index - 1].current_tok->value = strndup(input + *i + 1, nb - 1);
              info->lexer[*info->index - 1].current_tok->value[nb - 1] = '\0';
              break;
          default:
          if (*i < strlen(input))
          {
              
              struct cap_and_i *for_s = malloc(sizeof(struct cap_and_i));
              for_s->i = malloc(sizeof(size_t));
              for_s->cap = malloc(sizeof(size_t));
              *for_s->i = *i;
              *for_s->cap = *info->cap;
              for_s->mode = mode;
              for_s = spe_token(input, info->index, for_s, info->lexer);
              *i = *for_s->i;
              *info->cap = *for_s->cap;
              mode = for_s->mode;
              free(for_s->i);
              free(for_s->cap);
              free(for_s);
              nb = 0;
          }
              break;
        }
        info->lexer[*info->index].input = input;
        info->lexer[*info->index].pos = *i;
        *i = *i + nb;
        *info->index = *info->index + 1;
    }
    free(i);
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
    if (t == TOKEN_SIMPLE_QUOTE || t == TOKEN_WORDS || t == TOKEN_REDIR_SORTIE || t == TOKEN_REDIR_ENTREE 
    || t == TOKEN_REDIR_DESCRIPEUR || t == TOKEN_REDIR_FIN_FICHIER || t == TOKEN_DOUBLE_QUOTE
    || t == TOKEN_FOR_WORD || t == TOKEN_REDIR_PIPE
        || t == TOKEN_FOR_SINGLE_QUOTE 
        || t == TOKEN_FOR_DOUBLE_QUOTE
        || t == TOKEN_FOR_INT || t == TOKEN_REDIR_INPUT_DESCRIPEUR
        || t == TOKEN_REDIR_RW
    )
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
        || lexer[j + 1].current_tok->type == TOKEN_WORDS
        || lexer[j + 1].current_tok->type == TOKEN_REDIR_SORTIE 
        || lexer[j + 1].current_tok->type == TOKEN_REDIR_ENTREE 
        || lexer[j + 1].current_tok->type == TOKEN_REDIR_DESCRIPEUR
        || lexer[j + 1].current_tok->type == TOKEN_REDIR_FIN_FICHIER
        || lexer[j + 1].current_tok->type == TOKEN_DOUBLE_QUOTE
        || lexer[j + 1].current_tok->type == TOKEN_REDIR_INPUT_DESCRIPEUR
        || lexer[j + 1].current_tok->type == TOKEN_REDIR_RW
        || lexer[j + 1].current_tok->type == TOKEN_FOR_WORD
        || lexer[j + 1].current_tok->type == TOKEN_FOR_SINGLE_QUOTE
        || lexer[j + 1].current_tok->type == TOKEN_FOR_DOUBLE_QUOTE
        || lexer[j + 1].current_tok->type == TOKEN_FOR_INT
        || lexer[j + 1].current_tok->type == TOKEN_REDIR_PIPE)
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
    
    if (lexer[j].current_tok->value != NULL) {
        free(lexer[j].current_tok->value);
        lexer[j].current_tok->value = NULL;
      }
    
    if (t == TOKEN_SIMPLE_QUOTE || t == TOKEN_WORDS || t == TOKEN_REDIR_SORTIE || t == TOKEN_REDIR_ENTREE 
    || t == TOKEN_REDIR_DESCRIPEUR || t == TOKEN_REDIR_FIN_FICHIER || t == TOKEN_DOUBLE_QUOTE
    || t == TOKEN_FOR_WORD || t == TOKEN_REDIR_PIPE
    || t == TOKEN_REDIR_INPUT_DESCRIPEUR
    || t == TOKEN_REDIR_RW
    || t == TOKEN_FOR_SINGLE_QUOTE
    || t == TOKEN_FOR_DOUBLE_QUOTE
    || t == TOKEN_FOR_INT)
    {
      lexer[j].current_tok->value = test;
    }
    lexer[j].current_tok->type = t;
    return lexer[j].current_tok;
}
