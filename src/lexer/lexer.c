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

struct cap_and_i * spe_token(const char *input, size_t *index, struct cap_and_i *for_s, struct lexer *new)
{
  size_t nb = 0;
  size_t *i = for_s->i;
  size_t len = strlen(input);
  char current_char = input[*i];
  if (current_char >= 'A' && current_char <= 'Z')
    current_char = current_char + ('a' - 'A');
  if (current_char >= 'a' && current_char <= 'z')
  {
    /*
     * testing if
     */
      if (current_char == 'i' &&  *for_s->i + 1 < len && input[*for_s->i + 1] == 'f')
      {
        new[*index - 1].current_tok = token_new(TOKEN_IF);
        new[*index - 1].current_tok->value = NULL;
        nb = 1;
      }
      /*
       * testing else
       */
      else if (current_char == 'e' &&  *for_s->i + 3 < len && input[*for_s->i + 1] == 'l'
                && input[*for_s->i + 2] == 's' && input[*for_s->i + 3] == 'e')
      {
        new[*index - 1].current_tok = token_new(TOKEN_ELSE);
        new[*index - 1].current_tok->value = NULL;
        nb = 3;
      }
      /*
       * testing elif
       */
      else if (current_char == 'e' &&  *for_s->i + 3 < len && input[*for_s->i + 1] == 'l'
                && input[*for_s->i + 2] == 'i' && input[*for_s->i + 3] == 'f')
      {
        new[*index - 1].current_tok = token_new(TOKEN_ELIF);
        new[*index - 1].current_tok->value = NULL;
        nb = 3;
      }
      /*
       * testing then
       */
       else if (current_char == 't' &&  *for_s->i + 3 < len && input[*for_s->i + 1] == 'h'
                 && input[*for_s->i + 2] == 'e' && input[*for_s->i + 3] == 'n')
       {
         new[*index - 1].current_tok = token_new(TOKEN_THEN);
         new[*index - 1].current_tok->value = NULL;
         nb = 3;
       }
       /*
        * testing fi
        */
        else if (current_char == 'f' &&  *for_s->i + 1 < len && input[*for_s->i + 1] == 'i')
        {
          new[*index - 1].current_tok = token_new(TOKEN_FI);
          new[*index - 1].current_tok->value = NULL;
          nb = 1;
        }
       /*
        * testing for
        */
        else if (current_char == 'f' &&  *for_s->i + 2 < len && input[*for_s->i + 1] == 'o'
        && input[*for_s->i + 2] == 'r')
        {
          new[*index - 1].current_tok = token_new(TOKEN_FOR);
          new[*index - 1].current_tok->value = NULL;
          nb = 2;
        }
       /*
        * testing in
        */
        else if (current_char == 'i' &&  *for_s->i + 1 < len && input[*for_s->i + 1] == 'n')
        {
          new[*index - 1].current_tok = token_new(TOKEN_IN);
          new[*index - 1].current_tok->value = NULL;
          *index = *index + 1;
          *for_s->i = *for_s->i + 2;
          get_for(input, for_s, index, new);
          *index = *index - 1;
          *for_s->i = *for_s->i - 1;
          //printf("index here is %zu and i is %zu \n", *index, *for_s->i);
        }
       /*
        * testing Until
        */
       else if (current_char == 'u' &&  *for_s->i + 4 < len 
       && input[*for_s->i + 1] == 'n' && input[*for_s->i + 2] == 't'
       && input[*for_s->i + 3] == 'i' && input[*for_s->i + 4] == 'l')
        {
          new[*index - 1].current_tok = token_new(TOKEN_UNTIL);
          new[*index - 1].current_tok->value = NULL;
          nb = 4;
        }
       /*
        * testing Done
        */
       else if (current_char == 'd' &&  *for_s->i + 3 < len 
       && input[*for_s->i + 1] == 'o' && input[*for_s->i + 2] == 'n'
       && input[*for_s->i + 3] == 'e')
        {
          new[*index - 1].current_tok = token_new(TOKEN_DONE);
          new[*index - 1].current_tok->value = NULL;
          nb = 3;
        }
       /*
        * testing Do
        */
       else if (current_char == 'd' &&  *for_s->i + 1 < len && input[*for_s->i + 1] == 'o' )
        {
          new[*index - 1].current_tok = token_new(TOKEN_DO);
          new[*index - 1].current_tok->value = NULL;
          nb = 1;
        }
       /*
        * testing While
        */
       else if (current_char == 'w' &&  *for_s->i + 4 < len 
       && input[*for_s->i + 1] == 'h' && input[*for_s->i + 2] == 'i'
       && input[*for_s->i + 3] == 'l' && input[*for_s->i + 4] == 'e')
        {
          new[*index - 1].current_tok = token_new(TOKEN_WHILE);
          new[*index - 1].current_tok->value = NULL;
          nb = 4;
        }
        else
        {
          /*
           * testing word
           */
          new[*index - 1].current_tok = token_new(TOKEN_WORDS);
          nb = 0;
          while (*for_s->i + nb < strlen(input) && ((input[*for_s->i + nb] >= 'a'
                && input[*for_s->i + nb] <= 'z') ||
                (input[*for_s->i + nb] >= 'A' && input[*for_s->i + nb] <= 'Z')))
          {
              nb++;
          }
          new[*index - 1].current_tok->value = strndup(input + *for_s->i, nb);
          new[*index - 1].current_tok->value[nb] = '\0';
          if (nb > 0) {
            nb--;
          }
        }
  }
  else
  {
    if (!is_end(input[*for_s->i]))
    {
      if(input[*for_s->i] == '&' && *for_s->i + 1 < strlen(input) && input[*for_s->i + 1] == '&')
      {
        new[*index - 1].current_tok = token_new(TOKEN_AND);
        new[*index - 1].current_tok->value = NULL;
        nb++;
      }
      else
      {
        new[*index - 1].current_tok = token_new(TOKEN_WORDS);
        nb = 0;
        while (*for_s->i + nb < strlen(input) && !is_end(input[*for_s->i + nb]))
        {
          nb++;
        }
        new[*index - 1].current_tok->value = strndup(input + *for_s->i, nb);
        new[*index - 1].current_tok->value[nb] = '\0';
        if (nb > 0) {
          nb--;
        }
      }
    }
    //new[*index - 1].current_tok = token_new(TOKEN_ERROR);
    //fprintf(stderr, "error in token\n");
  }
  *for_s->i = *for_s->i + nb;
  return for_s;
}

struct lexer *lexer_new(const char *input)
{
    size_t *cap = malloc(sizeof(size_t));
    *cap = 20;
    struct lexer *new = malloc(20 * sizeof(struct lexer));
    size_t *i = malloc(sizeof(size_t));
    *i = 0;
    size_t *index = malloc(sizeof(size_t));
    *index = 1;
    size_t nb = 0;
    for (; *i <= strlen(input); *i = *i + 1)
    {
      nb = 0;
      if (is_space(input[*i]) || input[*i] == '[' || input[*i] == ']')
        continue;
      if (*cap < (*index + 2))
      {
          *cap *= 2;
          new = realloc(new, *cap * sizeof(struct lexer));
      }
      switch (input[*i])
      {
          case ('\n'):
              new[*index - 1].current_tok = token_new(TOKEN_LINE_BREAK);
              new[*index - 1].current_tok->value = NULL;
              break;
             /*
               * Testing for end of file
               */
          case ('\0'):
              new[*index - 1].current_tok = token_new(TOKEN_EOF);
              new[*index - 1].current_tok->value = NULL;
              //printf("end of file \n");
              break;
          case (';'):
              new[*index - 1].current_tok = token_new(TOKEN_SEMICOLON);
              new[*index - 1].current_tok->value = NULL;
              //printf("lol;;;;;lol\n");
              break;
              /*
               * Testing for pipeline
               */
          case ('|'):
              if (*i + 1 < strlen(input) && input[*i + 1] == '|')
              {
                new[*index - 1].current_tok = token_new(TOKEN_OR);
                new[*index - 1].current_tok->value = NULL;
                nb++;
              }
              else
              {
                new[*index - 1].current_tok = token_new(TOKEN_PIPE);
                new[*index - 1].current_tok->value = NULL;
              }
              break;
          case ('!'):
              new[*index - 1].current_tok = token_new(TOKEN_NEG);
              new[*index - 1].current_tok->value = NULL;
              break;
              /*
               * Testing for Redirections
               */
          case ('<'):
              if (*i + 1 < strlen(input) && input[*i + 1] == '&')
              {
                new[*index - 1].current_tok = token_new(TOKEN_REDIR_INPUT_DESCRIPEUR);
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
                  new[*index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                  new[*index - 1].current_tok->value[nb - tmp] = '\0';
                  nb--;
                }
                else
                {
                  char *tmp = malloc(2 * sizeof(char));
                  tmp[0] = '0';
                  tmp[1] = '\0';
                  new[*index - 1].current_tok->value = tmp;
                  nb--;
                }
              }
              else if (*i + 1 < strlen(input) && input[*i + 1] == '>')
              {
                new[*index - 1].current_tok = token_new(TOKEN_REDIR_RW);
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
                new[*index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                new[*index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              else
              {
                new[*index - 1].current_tok = token_new(TOKEN_REDIR_ENTREE);
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
                new[*index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                new[*index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              break;
              break;
          case ('>'):
              if (*i + 1 < strlen(input) && input[*i + 1] == '&')
              {
                new[*index - 1].current_tok = token_new(TOKEN_REDIR_DESCRIPEUR);
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
                  new[*index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                  new[*index - 1].current_tok->value[nb - tmp] = '\0';
                  nb--;
                }
                else
                {
                  char *tmp = malloc(2 * sizeof(char));
                  tmp[0] = '1';
                  tmp[1] = '\0';
                  new[*index - 1].current_tok->value = tmp;
                  nb--;
                }
              }
              else if (*i + 1 < strlen(input) && input[*i + 1] == '>')
              {
                new[*index - 1].current_tok = token_new(TOKEN_REDIR_FIN_FICHIER);
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
                new[*index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                new[*index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              else if (*i + 1 < strlen(input) && input[*i + 1] == '|')
              {
                new[*index - 1].current_tok = token_new(TOKEN_REDIR_PIPE);
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
                new[*index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                new[*index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              else
              {
                new[*index - 1].current_tok = token_new(TOKEN_REDIR_SORTIE);
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
                new[*index - 1].current_tok->value = strndup(input + *i + tmp, nb);
                new[*index - 1].current_tok->value[nb - tmp] = '\0';
                nb--;
              }
              break;
          case ('\''):
              new[*index - 1].current_tok = token_new(TOKEN_SIMPLE_QUOTE);
              nb = 1;
              while (*i + nb < strlen(input) && input[*i + nb] != '\0'
              && input[*i + nb] != '\'')
              {
                nb++;
              }
              new[*index - 1].current_tok->value = strndup(input + *i + 1, nb - 1);
              new[*index - 1].current_tok->value[nb - 1] = '\0';
              break;
          case ('\"'):
              new[*index - 1].current_tok = token_new(TOKEN_DOUBLE_QUOTE);
              nb = 1;
              while (*i + nb < strlen(input) && input[*i + nb] != '\0'
              && input[*i + nb] != '\"')
              {
                nb++;
              }
              new[*index - 1].current_tok->value = strndup(input + *i + 1, nb - 1);
              new[*index - 1].current_tok->value[nb - 1] = '\0';
              break;
          default:
          if (*i < strlen(input))
          {
              //printf("here\n");
              struct cap_and_i *for_s = malloc(sizeof(struct cap_and_i));
              for_s->i = malloc(sizeof(size_t));
              for_s->cap = malloc(sizeof(size_t));
              *for_s->i = *i;
              *for_s->cap = *cap;
              //printf("before input[i]=%c\n", input[*for_s->i]);
              for_s = spe_token(input, index, for_s, new);
              //printf("after input[i]=%c\n", input[*for_s->i]);
              *i = *for_s->i;
              *cap = *for_s->cap;
              free(for_s->i);
              free(for_s->cap);
              free(for_s);
              nb = 0;
          }
              break;
        }
        new[*index].input = input;
        new[*index].pos = *i;
        *i = *i + nb;
        *index = *index + 1;
    }
    free(index);
    free(i);
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
    //printf("start to pop\n");
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
    if (t == TOKEN_SIMPLE_QUOTE || t == TOKEN_WORDS || t == TOKEN_REDIR_SORTIE || t == TOKEN_REDIR_ENTREE 
    || t == TOKEN_REDIR_DESCRIPEUR || t == TOKEN_REDIR_FIN_FICHIER || t == TOKEN_DOUBLE_QUOTE
    || t == TOKEN_FOR_WORD || t == TOKEN_REDIR_PIPE
    || t == TOKEN_REDIR_INPUT_DESCRIPEUR
    || t == TOKEN_REDIR_RW
    || t == TOKEN_FOR_SINGLE_QUOTE
    || t == TOKEN_FOR_DOUBLE_QUOTE
    || t == TOKEN_FOR_INT)
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
