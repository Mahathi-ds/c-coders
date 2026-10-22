#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project.h"

#define MAX_TEXT 5000
#define MAX_SENTENCES 20
#define MAX_WORDS 500

int main(void)
{
    char text[MAX_TEXT];
    char sentences[MAX_SENTENCES][500];

    char *sentence_tokens[MAX_SENTENCES][MAX_WORDS];
    int sentence_token_count[MAX_SENTENCES];

    int frequency[MAX_SENTENCES * MAX_WORDS];
    char *unique_words[MAX_SENTENCES * MAX_WORDS];

    int sentence_count;
    int total_words = 0;
    int unique_count = 0;

    int i, j, k;
    int ch;

    FILE *file;

    printf("\n========================================\n");
    printf("       NLP PROCESSING PIPELINE\n");
    printf("========================================\n");

    /* STEP 1: TEXT NORMALIZATION */

    file = fopen("examples/sample_text.txt", "r");

    if (file == NULL)
    {
        printf("Error: Could not open examples/sample_text.txt\n");
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

    text_normalization(text, DEFAULT_OPTIONS);

    printf("%s\n", text);

    /* STEP 2: SENTENCE SPLITTING */

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
    /* STEP 4: CASE NORMALIZATION */

    printf("\n========== STEP 4: CASE NORMALIZATION ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        printf("Sentence %d: ", i + 1);

        for (j = 0; j < sentence_token_count[i]; j++)
        {
            make_lower(sentence_tokens[i][j]);

            printf("%s", sentence_tokens[i][j]);

            if (j + 1 < sentence_token_count[i])
            {
                printf(" | ");
            }
        }

        printf("\n");
    }

    /* STEP 3: WORD TOKENIZATION */

    printf("\n========== STEP 3: WORD TOKENIZATION ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        struct word **tokens;

        tokens = word_tokenize(sentences[i]);

        sentence_token_count[i] = 0;

        if (tokens == NULL)
        {
            printf("Sentence %d: Tokenization failed.\n", i + 1);
            continue;
        }

        printf("Sentence %d: ", i + 1);

        j = 0;

        while (tokens[j] != NULL && j < MAX_WORDS)
        {
            printf("%s", tokens[j]->data);

            if (tokens[j + 1] != NULL && j + 1 < MAX_WORDS)
            {
                printf(" | ");
            }

            j++;
        }

        printf("\n");

        sentence_token_count[i] = j;

        /*
           Copy token text into our own arrays so the following
           stages can process each sentence independently.
        */
        for (j = 0; j < sentence_token_count[i]; j++)
        {
            sentence_tokens[i][j] =
                malloc(strlen(tokens[j]->data) + 1);

            if (sentence_tokens[i][j] == NULL)
            {
                printf("Memory allocation failed.\n");
                return 1;
            }

            strcpy(sentence_tokens[i][j], tokens[j]->data);
        }

        total_words += sentence_token_count[i];

        /*
           The tokenizer's allocated memory is not freed here,
           because its ownership and cleanup API are not known.
        */
    }

    

    /* STEP 5: STOP-WORD REMOVAL */

    printf("\n========== STEP 5: STOP-WORD REMOVAL ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        int write_position = 0;

        /*
           Test each word against the stop-word list.
           This leaves the original stop_words() and
           remove_stopwords() implementations untouched.
        */
        char stopword_list[500][100];
        int stopword_count = stop_words(stopword_list);

        printf("Sentence %d: ", i + 1);

        for (j = 0; j < sentence_token_count[i]; j++)
        {
            int is_stopword = 0;

            for (k = 0; k < stopword_count; k++)
            {
                if (strcmp(sentence_tokens[i][j],
                           stopword_list[k]) == 0)
                {
                    is_stopword = 1;
                    break;
                }
            }

            if (!is_stopword)
            {
                sentence_tokens[i][write_position] =
                    sentence_tokens[i][j];

                write_position++;
            }
            else
            {
                free(sentence_tokens[i][j]);
            }
        }

        sentence_token_count[i] = write_position;

        for (j = 0; j < sentence_token_count[i]; j++)
        {
            printf("%s", sentence_tokens[i][j]);

            if (j + 1 < sentence_token_count[i])
            {
                printf(" | ");
            }
        }

        printf("\n");
    }

    /* STEP 6: WORD FREQUENCY ANALYSIS */

    printf("\n========== STEP 6: WORD FREQUENCY ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        for (j = 0; j < sentence_token_count[i]; j++)
        {
            int found = 0;

            for (k = 0; k < unique_count; k++)
            {
                if (strcmp(unique_words[k],
                           sentence_tokens[i][j]) == 0)
                {
                    frequency[k]++;
                    found = 1;
                    break;
                }
            }

            if (!found)
            {
                unique_words[unique_count] = sentence_tokens[i][j];
                frequency[unique_count] = 1;
                unique_count++;
            }
        }
    }

    for (i = 0; i < unique_count; i++)
    {
        printf("%s : %d\n", unique_words[i], frequency[i]);
    }
     /* STEP 8: STEMMING */

    printf("\n========== STEP 8: STEMMING ==========\n");

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

            free(sentence_tokens[i][j]);
        }

        printf("\n");
    }

    printf("\n========== PIPELINE COMPLETED ==========\n");

    return 0;
}

    /* STEP 7: BIGRAM GENERATION */

    printf("\n========== STEP 7: BIGRAM GENERATION (N = 2) ==========\n");

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

   