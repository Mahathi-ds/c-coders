
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

/* Function under test */
TokenList *nlp_generate_ngrams(TokenList *tokens, int n);

void free_token_list(TokenList *list)
{
    if (list == NULL)
        return;

    for (size_t i = 0; i < list->count; i++) {
        free(list->tokens[i]);
    }

    free(list->tokens);
    free(list);
}

void test_unigrams(void)
{
    char *words[] = {"I", "love", "NLP", "bangaram"};
    TokenList input = {4, words};

    TokenList *result = nlp_generate_ngrams(&input, 1);

    assert(result != NULL);
    assert(result->count == 4);
    assert(strcmp(result->tokens[0], "I") == 0);
    assert(strcmp(result->tokens[1], "love") == 0);
    assert(strcmp(result->tokens[2], "NLP") == 0);
    assert(strcmp(result->tokens[3], "bangaram") == 0);

    free_token_list(result);
    printf("PASS: unigrams\n");
}

void test_bigrams(void)
{
    char *words[] = {"I", "love", "NLP", "bangaram"};
    TokenList input = {4, words};

    TokenList *result = nlp_generate_ngrams(&input, 2);

    assert(result != NULL);
    assert(result->count == 3);
    assert(strcmp(result->tokens[0], "I love") == 0);
    assert(strcmp(result->tokens[1], "love NLP") == 0);
    assert(strcmp(result->tokens[2], "NLP bangaram") == 0);

    free_token_list(result);
    printf("PASS: bigrams\n");
}

void test_trigrams(void)
{
    char *words[] = {"I", "love", "NLP", "bangaram"};
    TokenList input = {4, words};

    TokenList *result = nlp_generate_ngrams(&input, 3);

    assert(result != NULL);
    assert(result->count == 2);
    assert(strcmp(result->tokens[0], "I love NLP") == 0);
    assert(strcmp(result->tokens[1], "love NLP bangaram") == 0);

    free_token_list(result);
    printf("PASS: trigrams\n");
}

void test_n_equals_token_count(void)
{
    char *words[] = {"hello", "world"};
    TokenList input = {2, words};

    TokenList *result = nlp_generate_ngrams(&input, 2);

    assert(result != NULL);
    assert(result->count == 1);
    assert(strcmp(result->tokens[0], "hello world") == 0);

    free_token_list(result);
    printf("PASS: n equals token count\n");
}

void test_n_greater_than_token_count(void)
{
    char *words[] = {"hello", "world"};
    TokenList input = {2, words};

    assert(nlp_generate_ngrams(&input, 3) == NULL);
    printf("PASS: n greater than token count\n");
}

void test_zero_n(void)
{
    char *words[] = {"hello", "world"};
    TokenList input = {2, words};

    assert(nlp_generate_ngrams(&input, 0) == NULL);
    printf("PASS: zero n\n");
}

void test_negative_n(void)
{
    char *words[] = {"hello", "world"};
    TokenList input = {2, words};

    assert(nlp_generate_ngrams(&input, -1) == NULL);
    printf("PASS: negative n\n");
}

void test_empty_tokens(void)
{
    TokenList input = {0, NULL};

    assert(nlp_generate_ngrams(&input, 1) == NULL);
    printf("PASS: empty token list\n");
}

int main(void)
{
    test_unigrams();
    test_bigrams();
    test_trigrams();
    test_n_equals_token_count();
    test_n_greater_than_token_count();
    test_zero_n();
    test_negative_n();
    test_empty_tokens();

    printf("\nAll tests passed!\n");
    return 0;
}
