
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

TokenList *nlp_generate_ngrams(TokenList *tokens, int n);

/* Free the generated N-gram list */
static void free_token_list(TokenList *list)
{
    if (list == NULL)
        return;

    for (size_t i = 0; i < list->count; i++)
        free(list->tokens[i]);

    free(list->tokens);
    free(list);
}

/* Generate and print N-grams */
static void show_ngrams(const char *title,
                        TokenList *input,
                        int n)
{
    printf("\n========== %s (n = %d) ==========\n",
           title, n);

    TokenList *result = nlp_generate_ngrams(input, n);

    if (result == NULL) {
        printf("No N-grams generated (NULL result).\n");
        return;
    }

    printf("Number of N-grams: %zu\n\n", result->count);

    for (size_t i = 0; i < result->count; i++) {
        printf("%zu. %s\n", i + 1, result->tokens[i]);
    }

    free_token_list(result);
}

int main(void)
{
    /* Normal test input */
    char *words[] = {
        "I", "love", "natural", "language", "processing"
    };

    size_t word_count = sizeof(words) / sizeof(words[0]);

    TokenList input = {
        words,
        word_count,
        word_count
    };

    printf("========== N-GRAM TESTING ==========\n");

    printf("\nInput tokens:\n");
    for (size_t i = 0; i < word_count; i++)
        printf("%s ", words[i]);
    printf("\n");

    /* Normal cases */
    show_ngrams("UNIGRAMS", &input, 1);
    show_ngrams("BIGRAMS", &input, 2);
    show_ngrams("TRIGRAMS", &input, 3);

    /* Boundary case: n equals token count */
    show_ngrams("N EQUALS TOKEN COUNT", &input, 5);

    /* Invalid cases */
    show_ngrams("INVALID N = 0", &input, 0);
    show_ngrams("INVALID N = -1", &input, -1);

    /* n is greater than the number of tokens */
    show_ngrams("N GREATER THAN TOKEN COUNT", &input, 6);

    /* Empty input */
    TokenList empty_input = {NULL, 0, 0};
    show_ngrams("EMPTY INPUT", &empty_input, 1);

    /* NULL input */
    printf("\n========== NULL INPUT ==========\n");
    TokenList *null_result = nlp_generate_ngrams(NULL, 2);

    if (null_result == NULL)
        printf("No result for NULL input.\n");
    else {
        printf("Unexpected result returned.\n");
        free_token_list(null_result);
    }

    /* Large input: 1000 tokens */
    printf("\n========== LARGE INPUT ==========\n");

    const size_t large_count = 1000;
    char **large_words = malloc(large_count * sizeof(char *));

    if (large_words == NULL) {
        printf("Unable to allocate large input.\n");
        return EXIT_FAILURE;
    }

    size_t allocated = 0;

    for (size_t i = 0; i < large_count; i++) {
        large_words[i] = malloc(32);

        if (large_words[i] == NULL) {
            printf("Unable to allocate token %zu.\n", i);
            break;
        }

        snprintf(large_words[i], 32, "word%zu", i + 1);
        allocated++;
    }

    if (allocated == large_count) {
        TokenList large_input = {
            large_words,
            large_count,
            large_count
        };

        TokenList *large_result =
            nlp_generate_ngrams(&large_input, 2);

        if (large_result != NULL) {
            printf("Input tokens: %zu\n", large_count);
            printf("Generated bigrams: %zu\n",
                   large_result->count);

            printf("First bigram: %s\n",
                   large_result->tokens[0]);

            printf("Last bigram: %s\n",
                   large_result->tokens[large_result->count - 1]);

            free_token_list(large_result);
        } else {
            printf("Unable to generate large N-grams.\n");
        }
    } else {
        printf("Large input was only partially allocated.\n");
    }

    /* Free all allocated input words */
    for (size_t i = 0; i < allocated; i++)
        free(large_words[i]);

    free(large_words);

    printf("\n========== TESTING FINISHED ==========\n");

    return EXIT_SUCCESS;
}