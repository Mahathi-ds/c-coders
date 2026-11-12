#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

#define MAX_TEXT 5000
#define MAX_WORDS 500

int main(void)
{
    char text[MAX_TEXT] = {0};
    FILE *file;
    int ch, i = 0;

    printf("\n====================================\n");
    printf("       NLP SENTIMENT TEST\n");
    printf("====================================\n");

    // STEP 1: READ INPUT TEXT

    file = fopen("examples/sentiment_sample.txt", "r");

    if (file == NULL)
    {
        printf("Cannot open sample_document.txt\n");
        return 1;
    }

    while (i < MAX_TEXT - 1 &&
           (ch = fgetc(file)) != EOF)
    {
        text[i++] = (char)ch;
    }

    text[i] = '\0';
    fclose(file);

    if (strlen(text) == 0)
    {
        printf("Input file is empty.\n");
        return 1;
    }

    printf("\nOriginal Text:\n%s\n", text);


    // STEP 2: TEXT NORMALIZATION

    printf("\n========== STEP 1: TEXT NORMALIZATION ==========\n");

    text_normalization(text, PUNCTUATION_REMOVAL_FALSE);

    printf("%s\n", text);


    // STEP 3: CASE NORMALIZATION

    printf("\n========== STEP 2: CASE NORMALIZATION ==========\n");

    make_lower(text);

    printf("%s\n", text);


    // STEP 4: WORD TOKENIZATION

    printf("\n========== STEP 3: WORD TOKENIZATION ==========\n");

    struct word *tokens = word_tokenize(text);

    if (tokens == NULL || rear <= 0)
    {
        printf("No tokens found.\n");
        return 1;
    }

    int token_count = rear;

    if (token_count > MAX_WORDS)
        token_count = MAX_WORDS;

    // Keep original and stemmed copies separately

    char **original_tokens = calloc(token_count, sizeof(char *));
    char **stemmed_tokens = calloc(token_count, sizeof(char *));

    if (original_tokens == NULL || stemmed_tokens == NULL)
    {
        printf("Memory allocation failed.\n");
        free(original_tokens);
        free(stemmed_tokens);
        return 1;
    }

    for (i = 0; i < token_count; i++)
    {
        original_tokens[i] = malloc(strlen(tokens[i].data) + 1);
        stemmed_tokens[i] = malloc(strlen(tokens[i].data) + 1);

        if (original_tokens[i] == NULL || stemmed_tokens[i] == NULL)
        {
            printf("Memory allocation failed.\n");

            for (int j = 0; j <= i; j++)
            {
                free(original_tokens[j]);
                free(stemmed_tokens[j]);
            }

            free(original_tokens);
            free(stemmed_tokens);
            return 1;
        }

        strcpy(original_tokens[i], tokens[i].data);
        strcpy(stemmed_tokens[i], tokens[i].data);

        printf("%s\n", original_tokens[i]);
    }


    // STEP 5: STEMMING

    printf("\n========== STEP 4: STEMMING ==========\n");

    for (i = 0; i < token_count; i++)
    {
        printf("%s -> ", stemmed_tokens[i]);

        nlp_stem_word(stemmed_tokens[i]);

        printf("%s\n", stemmed_tokens[i]);
    }


    // STEP 6: SENTIMENT SCORING

    printf("\n========== STEP 5: SENTIMENT SCORING ==========\n");

    TokenList token_list;

    token_list.tokens = original_tokens;
    token_list.count = (size_t)token_count;
    token_list.capacity = (size_t)token_count;

    SentimentResult result;

    int status = sentiment_analyze(&token_list, &result);

    if (status != 0)
    {
        printf("Sentiment analysis failed. Error: %d\n", status);
    }
    else
    {
        printf("\nPositive Count : %d\n", result.positive_count);
        printf("Negative Count : %d\n", result.negative_count);
        printf("Sentiment Score: %.4f\n", result.score);

        printf("\nFinal Sentiment: ");

        if (result.sentiment == SENTIMENT_POSITIVE)
            printf("POSITIVE\n");
        else if (result.sentiment == SENTIMENT_NEGATIVE)
            printf("NEGATIVE\n");
        else
            printf("NEUTRAL\n");
    }


    // FREE MEMORY

    for (i = 0; i < token_count; i++)
    {
        free(original_tokens[i]);
        free(stemmed_tokens[i]);
    }

    free(original_tokens);
    free(stemmed_tokens);

    printf("\n====================================\n");
    printf("       TEST COMPLETED\n");
    printf("====================================\n");

    return 0;
}