#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

#define MAX_TEXT 5000
#define MAX_SENTENCES 20
#define MAX_WORDS 500

int main(void)
{
    char text[MAX_TEXT];
    char sentences[MAX_SENTENCES][500];

    char *sentence_tokens[MAX_SENTENCES][MAX_WORDS];
    int sentence_token_count[MAX_SENTENCES];

    char *unique_words[MAX_SENTENCES * MAX_WORDS];
    int frequency[MAX_SENTENCES * MAX_WORDS];

    int sentence_count;
    int unique_count = 0;
    int i, j, k, ch;

    FILE *file;

    printf("\n========================================\n");
    printf("         NLP PROCESSING PIPELINE\n");
    printf("========================================\n");

    /* STEP 1: READ AND NORMALIZE THE COMPLETE TEXT */

    file = fopen("examples/sample_document.txt", "r");

    if (file == NULL)
    {
        printf("Error: Could not open sample_text.txt\n");
        return 1;
    }

    i = 0;

    while (i < MAX_TEXT - 1 && (ch = fgetc(file)) != EOF)
    {
        text[i] = (char)ch;
        i++;
    }

    text[i] = '\0';
    fclose(file);

    printf("\n========== STEP 1: TEXT NORMALIZATION ==========\n");

    text_normalization(text, PUNCTUATION_REMOVAL_FALSE);

    printf("%s\n", text);

    /* STEP 2: SPLIT THE NORMALIZED TEXT INTO SENTENCES */

    printf("\n========== STEP 2: SENTENCE SPLITTING ==========\n");

    sentence_count = sentence_split(
        text, sentences, MAX_SENTENCES
    );

    if (sentence_count <= 0)
    {
        printf("No sentences found.\n");
        return 0;
    }

    for (i = 0; i < sentence_count; i++)
    {
        printf("Sentence %d: %s\n", i + 1, sentences[i]);
    }

    /* STEP 3: CASE NORMALIZATION OF EACH SENTENCE */

    printf("\n========== STEP 3: CASE NORMALIZATION ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        make_lower(sentences[i]);
        printf("Sentence %d: %s\n", i + 1, sentences[i]);
    }

    
    /* STEP 4: TOKENIZE EACH SENTENCE */

    printf("\n========== STEP 4: WORD TOKENIZATION ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        struct word *tokens;
        int count = 0;

        sentence_token_count[i] = 0;

        tokens = word_tokenize(sentences[i]);

        if (tokens == NULL)
        {
            printf("Sentence %d: Tokenization failed.\n", i + 1);
            continue;
        }

        // Get the number of tokens generated
        count = rear;

        printf("Sentence %d: ", i + 1);

        for (j = 0; j < count && j < MAX_WORDS; j++)
        {
            sentence_tokens[i][j] =
                malloc(strlen(tokens[j].data) + 1);

            if (sentence_tokens[i][j] == NULL)
            {
                printf("\nMemory allocation failed.\n");
                return 1;
            }

            strcpy(sentence_tokens[i][j], tokens[j].data);

            printf("%s", sentence_tokens[i][j]);

            if (j + 1 < count && j + 1 < MAX_WORDS)
            {
                printf(" | ");
            }
        }

        // Store the actual number of tokens copied
        sentence_token_count[i] = (count < MAX_WORDS) ? count : MAX_WORDS;

        printf("\n");
    }


   
    
/* STEP 5: STOP-WORD REMOVAL */

printf("\n========== STEP 5: STOP-WORD REMOVAL ==========\n");

int keep_negations = 1;

for (i = 0; i < sentence_count; i++)
{
    struct word temp_tokens[MAX_WORDS];

    int old_count = sentence_token_count[i];

    // Convert char* tokens into struct word tokens
    for (j = 0; j < old_count; j++)
    {
        strcpy(temp_tokens[j].data, sentence_tokens[i][j]);
    }

    // Set rear to the number of tokens in this sentence
    rear = old_count;

    // Call the actual library function
    int status = remove_stopwords(temp_tokens, keep_negations);

    if (status == -1)
    {
        printf("Stop-word removal failed for sentence %d.\n", i + 1);
        continue;
    }

    // Copy the remaining tokens back into sentence_tokens
    for (j = 0; j < rear; j++)
    {
        strcpy(sentence_tokens[i][j], temp_tokens[j].data);
    }

    // Free the strings corresponding to removed tokens
    for (j = rear; j < old_count; j++)
    {
        free(sentence_tokens[i][j]);
        sentence_tokens[i][j] = NULL;
    }

    sentence_token_count[i] = rear;

    printf("Sentence %d: ", i + 1);

    for (j = 0; j < sentence_token_count[i]; j++)
    {
        printf("%s", sentence_tokens[i][j]);

        if (j + 1 < sentence_token_count[i])
            printf(" | ");
    }

    printf("\n");
}


    /* STEP 6: WORD FREQUENCY ANALYSIS */

printf("\n========== STEP 6: WORD FREQUENCY ==========\n");

TokenList all_tokens;
FreqList *freq_list;

char *combined_tokens[MAX_SENTENCES * MAX_WORDS];
int total_tokens = 0;

/* Collect remaining tokens from all sentences */
for (i = 0; i < sentence_count; i++)
{
    for (j = 0; j < sentence_token_count[i]; j++)
    {
        combined_tokens[total_tokens] = sentence_tokens[i][j];
        total_tokens++;
    }
}

/* Create one TokenList containing all remaining words */
all_tokens.tokens = combined_tokens;
all_tokens.count = (size_t)total_tokens;

/* Use the frequency.c implementation */
freq_list = nlp_word_frequency(&all_tokens);

if (freq_list == NULL)
{
    printf("Word frequency calculation failed.\n");
}
else
{
    for (i = 0; i < (int)freq_list->count; i++)
    {
        printf("%s : %d\n",
               freq_list->frequencies[i].word,
               freq_list->frequencies[i].count);
    }

    free(freq_list->frequencies);
    free(freq_list);
}
    /* STEP 7: STEM THE REMAINING WORDS */

    printf("\n========== STEP 7: STEMMING ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        printf("Sentence %d: ", i + 1);

        for (j = 0; j < sentence_token_count[i]; j++)
        {
            nlp_stem_word(sentence_tokens[i][j]);

            printf("%s", sentence_tokens[i][j]);

            if (j + 1 < sentence_token_count[i])
            {
                printf(" | ");
            }
        }

        printf("\n");
    }

    /* STEP 8: GENERATE BIGRAMS FROM STEMMED WORDS */

    printf("\n========== STEP 8: BIGRAM GENERATION (N = 2) ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        TokenList token_list;
        TokenList *bigrams;

        token_list.tokens = sentence_tokens[i];
        token_list.count = (size_t)sentence_token_count[i];

        printf("Sentence %d:\n", i + 1);

        bigrams = nlp_generate_ngrams(&token_list, 2);

        if (bigrams == NULL)
        {
            printf("Not enough words to generate bigrams.\n");
            continue;
        }

        for (j = 0; j < (int)bigrams->count; j++)
        {
            printf("%s\n", bigrams->tokens[j]);
            free(bigrams->tokens[j]);
        }

        free(bigrams->tokens);
        free(bigrams);
    }

    /* RELEASE THE COPIED TOKENS */

    for (i = 0; i < sentence_count; i++)
    {
        for (j = 0; j < sentence_token_count[i]; j++)
        {
            free(sentence_tokens[i][j]);
        }
    }

    printf("\n========== PIPELINE COMPLETED ==========\n");

    return 0;
}