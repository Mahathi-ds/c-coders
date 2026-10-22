
#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

TokenList *nlp_generate_ngrams(TokenList *tokens, int n)
{
    /* Validate input */
    if (tokens == NULL || n <= 0)
        return NULL;

    if (tokens->count > 0 && tokens->tokens == NULL)
        return NULL;

    /* Not enough tokens to form an n-gram */
    if ((size_t)n > tokens->count)
        return NULL;

    /* Validate each input token */
    for (size_t i = 0; i < tokens->count; i++) {
        if (tokens->tokens[i] == NULL)
            return NULL;
    }

    /* Allocate the output TokenList */
    TokenList *ngram_list = malloc(sizeof(TokenList));

    if (ngram_list == NULL)
        return NULL;

    ngram_list->count = tokens->count - (size_t)n + 1;
    ngram_list->tokens = NULL;

    /* Allocate the array of output strings */
    ngram_list->tokens =
        calloc(ngram_list->count, sizeof(char *));

    if (ngram_list->tokens == NULL) {
        free(ngram_list);
        return NULL;
    }

    /* Generate each n-gram */
    for (size_t i = 0; i < ngram_list->count; i++) {

        size_t required_size = 1; /* Null terminator */

        /* Calculate the exact required string size */
        for (int j = 0; j < n; j++) {
            size_t word_len =
                strlen(tokens->tokens[i + (size_t)j]);

            /* Check for size overflow */
            if (word_len > (size_t)-1 - required_size) {
                goto cleanup;
            }

            required_size += word_len;

            /* Add space between words */
            if (j < n - 1) {
                if (required_size == (size_t)-1)
                    goto cleanup;

                required_size++;
            }
        }

        /* Allocate enough space for this n-gram */
        ngram_list->tokens[i] = malloc(required_size);

        if (ngram_list->tokens[i] == NULL)
            goto cleanup;

        /* Build the n-gram safely */
        char *dest = ngram_list->tokens[i];

        for (int j = 0; j < n; j++) {
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
    /* Release all allocated memory on failure */
    for (size_t i = 0; i < ngram_list->count; i++)
        free(ngram_list->tokens[i]);

    free(ngram_list->tokens);
    free(ngram_list);

    return NULL;
}
