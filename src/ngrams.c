#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

/* Generate n-grams */
TokenList *nlp_generate_ngrams(TokenList *tokens, int n)
{
    if (tokens == NULL || n <= 0)
        return NULL;

    if (tokens->count > 0 && tokens->tokens == NULL)
        return NULL;

    if ((size_t)n > tokens->count)
        return NULL;

    for (size_t i = 0; i < tokens->count; i++)
    {
        if (tokens->tokens[i] == NULL)
            return NULL;
    }

    TokenList *ngram_list = malloc(sizeof(TokenList));

    if (ngram_list == NULL)
        return NULL;

    ngram_list->count = tokens->count - (size_t)n + 1;
    ngram_list->capacity = ngram_list->count;

    ngram_list->tokens =
        calloc(ngram_list->count, sizeof(char *));

    if (ngram_list->tokens == NULL)
    {
        free(ngram_list);
        return NULL;
    }

    for (size_t i = 0; i < ngram_list->count; i++)
    {
        size_t required_size = 1;

        for (int j = 0; j < n; j++)
        {
            size_t word_len =
                strlen(tokens->tokens[i + (size_t)j]);

            if (word_len > (size_t)-1 - required_size)
                goto cleanup;

            required_size += word_len;

            if (j < n - 1)
                required_size++;
        }

        ngram_list->tokens[i] = malloc(required_size);

        if (ngram_list->tokens[i] == NULL)
            goto cleanup;

        char *dest = ngram_list->tokens[i];

        for (int j = 0; j < n; j++)
        {
            const char *word =
                tokens->tokens[i + (size_t)j];

            size_t word_len = strlen(word);

            memcpy(dest, word, word_len);
            dest += word_len;

            if (j < n - 1)
                *dest++ = ' ';
        }

        *dest = '\0';
    }

    return ngram_list;

cleanup:

    for (size_t i = 0; i < ngram_list->count; i++)
        free(ngram_list->tokens[i]);

    free(ngram_list->tokens);
    free(ngram_list);

    return NULL;
}


/* Free dynamically allocated TokenList */
void nlp_token_list_free(TokenList *list)
{
    if (list == NULL)
        return;

    if (list->tokens != NULL)
    {
        for (size_t i = 0; i < list->count; i++)
        {
            free(list->tokens[i]);
        }

        free(list->tokens);
    }

    free(list);
}