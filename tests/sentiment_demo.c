// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// #include "project.h"

// /* Dynamically read entire file */
// static char *read_file(const char *filename)
// {
//     FILE *fp = fopen(filename, "rb");

//     if (fp == NULL)
//         return NULL;

//     size_t capacity = 4096;
//     size_t length = 0;

//     char *text = malloc(capacity);

//     if (text == NULL) {
//         fclose(fp);
//         return NULL;
//     }

//     size_t bytes;

//     while (1) {

//         if (length + 2048 + 1 > capacity) {

//             if (capacity > (size_t)-1 / 2) {
//                 free(text);
//                 fclose(fp);
//                 return NULL;
//             }

//             size_t new_capacity = capacity * 2;

//             char *temp = realloc(text, new_capacity);

//             if (temp == NULL) {
//                 free(text);
//                 fclose(fp);
//                 return NULL;
//             }

//             text = temp;
//             capacity = new_capacity;
//         }

//         bytes = fread(
//             text + length,
//             1,
//             capacity - length - 1,
//             fp
//         );

//         length += bytes;

//         if (bytes == 0) {
//             if (ferror(fp)) {
//                 free(text);
//                 fclose(fp);
//                 return NULL;
//             }

//             break;
//         }
//     }

//     text[length] = '\0';

//     fclose(fp);

//     return text;
// }


// /* Dynamically add token to TokenList */
// static int add_token(TokenList *tokens, const char *start,
//                      size_t length)
// {
//     if (tokens->count == tokens->capacity) {

//         size_t new_capacity =
//             tokens->capacity == 0 ? 16 : tokens->capacity * 2;

//         if (new_capacity < tokens->capacity ||
//             new_capacity > (size_t)-1 / sizeof(char *))
//             return -1;

//         char **temp = realloc(
//             tokens->tokens,
//             new_capacity * sizeof(char *)
//         );

//         if (temp == NULL)
//             return -1;

//         tokens->tokens = temp;
//         tokens->capacity = new_capacity;
//     }

//     char *word = malloc(length + 1);

//     if (word == NULL)
//         return -1;

//     memcpy(word, start, length);
//     word[length] = '\0';

//     tokens->tokens[tokens->count++] = word;

//     return 0;
// }


// /* Tokenize input text */
// static int tokenize_text(const char *text, TokenList *tokens)
// {
//     const char *start = NULL;

//     for (const char *p = text; ; p++) {

//         if (*p != '\0' && !isspace((unsigned char)*p)) {

//             if (start == NULL)
//                 start = p;

//         } else {

//             if (start != NULL) {

//                 size_t length = (size_t)(p - start);

//                 if (add_token(tokens, start, length) != 0)
//                     return -1;

//                 start = NULL;
//             }

//             if (*p == '\0')
//                 break;
//         }
//     }

//     return 0;
// }


// /* Free TokenList memory */
// static void free_tokens(TokenList *tokens)
// {
//     for (size_t i = 0; i < tokens->count; i++)
//         free(tokens->tokens[i]);

//     free(tokens->tokens);

//     tokens->tokens = NULL;
//     tokens->count = 0;
//     tokens->capacity = 0;
// }


// int main(void)
// {
//     char *text = read_file("sentiment_input.txt");

//     if (text == NULL) {
//         perror("Unable to read input file");
//         return 1;
//     }

//     TokenList tokens = {NULL, 0, 0};

//     if (tokenize_text(text, &tokens) != 0) {

//         printf("Tokenization failed\n");

//         free(text);
//         free_tokens(&tokens);

//         return 1;
//     }

//     free(text);

//     SentimentResult result;

//     int status = sentiment_analyze(&tokens, &result);

//     if (status != 0) {

//         printf("Sentiment analysis failed: %d\n", status);

//     } else {

//         printf("\n===== SENTIMENT ANALYSIS =====\n");

//         printf("Total tokens: %zu\n", tokens.count);

//         printf("Positive words: %d\n",
//                result.positive_count);

//         printf("Negative words: %d\n",
//                result.negative_count);

//         printf("Sentiment score: %.4f\n",
//                result.score);

//         if (result.sentiment == SENTIMENT_POSITIVE)
//             printf("Sentiment: POSITIVE\n");

//         else if (result.sentiment == SENTIMENT_NEGATIVE)
//             printf("Sentiment: NEGATIVE\n");

//         else
//             printf("Sentiment: NEUTRAL\n");
//     }

//     free_tokens(&tokens);

//     return status == 0 ? 0 : 1;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "project.h"

/* Test case structure */
typedef struct {
    const char *sentence;
    Sentiment expected;
} TestCase;


/* Add a token dynamically */
static int add_token(TokenList *tokens, const char *start, size_t length)
{
    if (tokens->count == tokens->capacity) {
        size_t new_capacity =
            tokens->capacity == 0 ? 16 : tokens->capacity * 2;

        if (new_capacity < tokens->capacity ||
            new_capacity > (size_t)-1 / sizeof(char *))
            return -1;

        char **temp = realloc(
            tokens->tokens,
            new_capacity * sizeof(char *)
        );

        if (temp == NULL)
            return -1;

        tokens->tokens = temp;
        tokens->capacity = new_capacity;
    }

    char *word = malloc(length + 1);

    if (word == NULL)
        return -1;

    memcpy(word, start, length);
    word[length] = '\0';

    tokens->tokens[tokens->count++] = word;

    return 0;
}


/* Tokenize words and keep punctuation as separate tokens.
   This allows sentiment.c to reset negation at sentence boundaries. */
static int tokenize_text(const char *text, TokenList *tokens)
{
    const char *p = text;

    while (*p != '\0') {

        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }

        const char *start = p;
        int punctuation = ispunct((unsigned char)*p);

        while (*p != '\0' &&
               !isspace((unsigned char)*p) &&
               ispunct((unsigned char)*p) == punctuation) {
            p++;
        }

        if (add_token(tokens, start, (size_t)(p - start)) != 0)
            return -1;
    }

    return 0;
}


/* Free token memory */
static void free_tokens(TokenList *tokens)
{
    for (size_t i = 0; i < tokens->count; i++)
        free(tokens->tokens[i]);

    free(tokens->tokens);

    tokens->tokens = NULL;
    tokens->count = 0;
    tokens->capacity = 0;
}


/* Convert sentiment enum to text */
static const char *sentiment_name(Sentiment sentiment)
{
    if (sentiment == SENTIMENT_POSITIVE)
        return "POSITIVE";

    if (sentiment == SENTIMENT_NEGATIVE)
        return "NEGATIVE";

    return "NEUTRAL";
}


/* All automated test cases */
static const TestCase test_cases[] = {

    /* Basic sentiment */
    {"I am happy", SENTIMENT_POSITIVE},
    {"This is terrible", SENTIMENT_NEGATIVE},
    {"This movie is amazing", SENTIMENT_POSITIVE},
    {"The service is poor", SENTIMENT_NEGATIVE},
    {"This is okay", SENTIMENT_NEUTRAL},
    {"I love this beautiful place", SENTIMENT_POSITIVE},

    /* Negation */
    {"I am not happy", SENTIMENT_NEGATIVE},
    {"This is not bad", SENTIMENT_POSITIVE},
    {"This is not good", SENTIMENT_NEGATIVE},
    {"This is not terrible", SENTIMENT_POSITIVE},
    {"I am not sad", SENTIMENT_POSITIVE},
    {"This is not horrible", SENTIMENT_POSITIVE},
    {"I am not very happy", SENTIMENT_NEGATIVE},
    {"This is not very bad", SENTIMENT_POSITIVE},

    /* Intensity */
    {"This is very good", SENTIMENT_POSITIVE},
    {"This is extremely good", SENTIMENT_POSITIVE},
    {"This is really amazing", SENTIMENT_POSITIVE},
    {"This is very bad", SENTIMENT_NEGATIVE},
    {"This is extremely horrible", SENTIMENT_NEGATIVE},

    /* Mixed sentiment */
    {"The movie is good but the ending is bad", SENTIMENT_NEUTRAL},
    {"I love the design but hate the price", SENTIMENT_NEUTRAL},
    {"The food is excellent but service is poor", SENTIMENT_NEUTRAL},
    {"Good quality and wonderful experience", SENTIMENT_POSITIVE},
    {"Terrible service and horrible food", SENTIMENT_NEGATIVE},
    {"Good good bad", SENTIMENT_POSITIVE},
    {"Bad bad good", SENTIMENT_NEGATIVE},

    /* Punctuation and sentence boundaries */
    {"Good!!!", SENTIMENT_POSITIVE},
    {"Bad???", SENTIMENT_NEGATIVE},
    {"Good. Bad.", SENTIMENT_NEUTRAL},
    {"I am not happy. I am excited.", SENTIMENT_NEUTRAL},

    /* Edge cases */
    {"", SENTIMENT_NEUTRAL},
    {"     ", SENTIMENT_NEUTRAL},
    {"12345 67890", SENTIMENT_NEUTRAL},
    {"xyz abc pqr", SENTIMENT_NEUTRAL},
    {"@#$%^&", SENTIMENT_NEUTRAL}
};


int main(void)
{
    int passed = 0;
    int failed = 0;

    size_t total_tests =
        sizeof(test_cases) / sizeof(test_cases[0]);

    printf("\n");
    printf("====================================================\n");
    printf("          SENTIMENT ANALYSIS TESTING\n");
    printf("====================================================\n");

    for (size_t i = 0; i < total_tests; i++) {

        TokenList tokens = {NULL, 0, 0};

        SentimentResult result;

        printf("\nTest Case %zu\n", i + 1);

        printf("Sentence : \"%s\"\n",
               test_cases[i].sentence);

        /* Tokenization */
        if (tokenize_text(test_cases[i].sentence, &tokens) != 0) {

            printf("ERROR: Tokenization failed\n");

            free_tokens(&tokens);
            failed++;
            continue;
        }

        /* Sentiment analysis */
        int status = sentiment_analyze(&tokens, &result);

        if (status != 0) {

            printf("ERROR: Sentiment analysis failed (%d)\n", status);

            failed++;
            free_tokens(&tokens);
            continue;
        }

        /* Display results */
        printf("Tokens   : %zu\n", tokens.count);

        printf("Positive : %d\n",
               result.positive_count);

        printf("Negative : %d\n",
               result.negative_count);

        printf("Score    : %+.4f\n",
               result.score);

        printf("Expected : %s\n",
               sentiment_name(test_cases[i].expected));

        printf("Actual   : %s\n",
               sentiment_name(result.sentiment));

        /* Compare expected and actual results */
        if (result.sentiment == test_cases[i].expected) {

            printf("Result   : PASS\n");
            passed++;

        } else {

            printf("Result   : FAIL\n");
            failed++;
        }

        free_tokens(&tokens);
    }

    /* Final test summary */
    printf("\n");
    printf("====================================================\n");
    printf("                 TEST SUMMARY\n");
    printf("====================================================\n");

    printf("Total tests : %zu\n", total_tests);
    printf("Passed      : %d\n", passed);
    printf("Failed      : %d\n", failed);

    if (failed == 0)
        printf("\nAll test cases passed!\n");
    else
        printf("\nSome test cases failed. Check the output above.\n");

    printf("====================================================\n");

    return failed == 0 ? 0 : 1;
}
