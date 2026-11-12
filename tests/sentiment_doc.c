
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "../include/project.h"

#define MAX_TEXT 5000
#define MAX_WORDS 500

#define INPUT_FILE    "examples/sample_document.txt"
#define POSITIVE_FILE "data/positive_words.txt"
#define NEGATIVE_FILE "data/negative_words.txt"

/* Check that the dictionary files required by sentiment.c are available.
   sentiment_analyze() loads their contents internally. */
static int check_dictionary_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        return 0;

    fclose(fp);
    return 1;
}

static void free_token_copies(char **original_tokens,
                              char **stemmed_tokens,
                              int count)
{
    if (original_tokens != NULL) {
        for (int i = 0; i < count; i++)
            free(original_tokens[i]);
    }

    if (stemmed_tokens != NULL) {
        for (int i = 0; i < count; i++)
            free(stemmed_tokens[i]);
    }

    free(original_tokens);
    free(stemmed_tokens);
}

int main(void)
{
    char text[MAX_TEXT] = {0};
    FILE *file;
    int ch;
    int i = 0;
    struct word *tokens = NULL;
    char **original_tokens = NULL;
    char **stemmed_tokens = NULL;
    int token_count = 0;

    printf("\n====================================\n");
    printf("       NLP SENTIMENT TEST\n");
    printf("====================================\n");

    /* STEP 1: CHECK DICTIONARY FILES
       The modified sentiment_analyze() reads these files automatically. */

    printf("\nLoading dictionaries from:\n");
    printf("  %s\n", POSITIVE_FILE);
    printf("  %s\n", NEGATIVE_FILE);

    if (!check_dictionary_file(POSITIVE_FILE)) {
        printf("Error: Cannot open %s\n", POSITIVE_FILE);
        printf("Run this program from the project root folder.\n");
        return 1;
    }

    if (!check_dictionary_file(NEGATIVE_FILE)) {
        printf("Error: Cannot open %s\n", NEGATIVE_FILE);
        printf("Run this program from the project root folder.\n");
        return 1;
    }

    /* STEP 2: READ INPUT TEXT DOCUMENT */

    file = fopen(INPUT_FILE, "r");

    if (file == NULL) {
        printf("\nCannot open input document: %s\n", INPUT_FILE);
        return 1;
    }

    while (i < MAX_TEXT - 1 && (ch = fgetc(file)) != EOF)
        text[i++] = (char)ch;

    text[i] = '\0';
    fclose(file);

    if (text[0] == '\0') {
        printf("\nInput file is empty: %s\n", INPUT_FILE);
        return 1;
    }

    printf("\nInput document: %s\n", INPUT_FILE);
    printf("\nOriginal Text:\n%s\n", text);

    /* STEP 3: TEXT NORMALIZATION */

    printf("\n========== STEP 1: TEXT NORMALIZATION ==========\n");

    text_normalization(text, PUNCTUATION_REMOVAL_FALSE);
    printf("%s\n", text);

    /* STEP 4: CASE NORMALIZATION */

    printf("\n========== STEP 2: CASE NORMALIZATION ==========\n");

    make_lower(text);
    printf("%s\n", text);

    /* STEP 5: WORD TOKENIZATION */

    printf("\n========== STEP 3: WORD TOKENIZATION ==========\n");

    tokens = word_tokenize(text);

    if (tokens == NULL || rear <= 0) {
        printf("No tokens found.\n");
        free(tokens);
        return 1;
    }

    token_count = rear;

    if (token_count > MAX_WORDS) {
        printf("Warning: Document has more than %d tokens; "
               "only the first %d will be processed.\n",
               MAX_WORDS, MAX_WORDS);
        token_count = MAX_WORDS;
    }

    original_tokens = calloc((size_t)token_count, sizeof(char *));
    stemmed_tokens = calloc((size_t)token_count, sizeof(char *));

    if (original_tokens == NULL || stemmed_tokens == NULL) {
        printf("Memory allocation failed.\n");
        free_token_copies(original_tokens, stemmed_tokens, token_count);
        free(tokens);
        return 1;
    }

    for (i = 0; i < token_count; i++) {
        size_t length = strlen(tokens[i].data);

        original_tokens[i] = malloc(length + 1);
        stemmed_tokens[i] = malloc(length + 1);

        if (original_tokens[i] == NULL || stemmed_tokens[i] == NULL) {
            printf("Memory allocation failed.\n");
            free_token_copies(original_tokens, stemmed_tokens, token_count);
            free(tokens);
            return 1;
        }

        strcpy(original_tokens[i], tokens[i].data);
        strcpy(stemmed_tokens[i], tokens[i].data);

        printf("%s\n", original_tokens[i]);
    }

    /* STEP 6: STEMMING
       Keep this as a demonstration only. Sentiment analysis below uses
       original_tokens so dictionary words are not changed by stemming. */

    printf("\n========== STEP 4: STEMMING ==========\n");

    for (i = 0; i < token_count; i++) {
        printf("%s -> ", stemmed_tokens[i]);
        nlp_stem_word(stemmed_tokens[i]);
        printf("%s\n", stemmed_tokens[i]);
    }

    /* STEP 7: SENTIMENT SCORING */

    printf("\n========== STEP 5: SENTIMENT SCORING ==========\n");

    TokenList token_list;
    token_list.tokens = original_tokens;
    token_list.count = (size_t)token_count;
    token_list.capacity = (size_t)token_count;

    SentimentResult result;
    int status = sentiment_analyze(&token_list, &result);

    if (status != 0) {
        printf("Sentiment analysis failed. Error: %d\n", status);
        if (status == -2)
            printf("Check that both dictionary files exist and are readable.\n");
    } else {
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

    /* FREE MEMORY */

    free_token_copies(original_tokens, stemmed_tokens, token_count);
    free(tokens);

    printf("\n====================================\n");
    printf("       TEST COMPLETED\n");
    printf("====================================\n");

    return (status == 0) ? 0 : 1;
}
