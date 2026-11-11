#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

#define MAX_TEXT 5000
#define MAX_SENTENCES 20
#define MAX_WORDS 500
#define MAX_KEYWORDS 5
#define TOTAL_CAPACITY (MAX_SENTENCES * MAX_WORDS)

int main(void)
{
    char text[MAX_TEXT] = {0};
    char sentences[MAX_SENTENCES][500] = {{0}};

    char *sentence_tokens[MAX_SENTENCES][MAX_WORDS] = {{0}};
    int sentence_token_count[MAX_SENTENCES] = {0};

    int sentence_count = 0;
    int i, j, ch;

    FILE *file;

    printf("\n========================================\n");
    printf("         NLP PROCESSING PIPELINE\n");
    printf("========================================\n");

    /* STEP 1: TEXT NORMALIZATION */

    printf("\n========== STEP 1: TEXT NORMALIZATION ==========\n");

    file = fopen("examples/sample_document.txt", "r");

    if (file == NULL)
    {
        printf("Error: Cannot open examples/sample_document.txt\n");
        return 1;
    }

    i = 0;

    while (i < MAX_TEXT - 1 && (ch = fgetc(file)) != EOF)
    {
        text[i++] = (char)ch;
    }

    text[i] = '\0';
    fclose(file);

    if (strlen(text) == 0)
    {
        printf("Input file is empty!\n");
        return 1;
    }

    printf("Before normalization:\n%s\n", text);

    text_normalization_n(
        text,
        sizeof(text),
        DEFAULT_OPTIONS
    );

    printf("\nAfter normalization:\n%s\n", text);

    if (strlen(text) == 0)
    {
        printf("Normalization produced empty text.\n");
        return 1;
    }

    /* STEP 2: SENTENCE SPLITTING */

    printf("\n========== STEP 2: SENTENCE SPLITTING ==========\n");

    sentence_count = sentence_split(
        text,
        sentences,
        MAX_SENTENCES
    );

    if (sentence_count <= 0)
    {
        printf("No sentences found.\n");
        return 1;
    }

    if (sentence_count > MAX_SENTENCES)
        sentence_count = MAX_SENTENCES;

    for (i = 0; i < sentence_count; i++)
    {
        printf("Sentence %d: %s\n", i + 1, sentences[i]);
    }

    /* STEP 3: CASE NORMALIZATION */

    printf("\n========== STEP 3: CASE NORMALIZATION ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        make_lower(sentences[i]);

        printf("Sentence %d: %s\n", i + 1, sentences[i]);
    }

    /* STEP 4: WORD TOKENIZATION */

    printf("\n========== STEP 4: WORD TOKENIZATION ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        struct word *tokens = word_tokenize(sentences[i]);

        int count = rear;

        if (tokens == NULL || count <= 0)
        {
            printf("Sentence %d: No tokens found.\n", i + 1);
            continue;
        }

        if (count > MAX_WORDS)
            count = MAX_WORDS;

        for (j = 0; j < count; j++)
        {
            sentence_tokens[i][j] =
                malloc(strlen(tokens[j].data) + 1);

            if (sentence_tokens[i][j] == NULL)
            {
                printf("Memory allocation failed.\n");
                goto cleanup;
            }

            strcpy(sentence_tokens[i][j], tokens[j].data);

            sentence_token_count[i]++;
        }

        printf("Sentence %d: ", i + 1);

        for (j = 0; j < sentence_token_count[i]; j++)
        {
            printf("%s", sentence_tokens[i][j]);

            if (j + 1 < sentence_token_count[i])
                printf(" | ");
        }

        printf("\n");
    }

    /* STEP 5: STOP-WORD REMOVAL */

    printf("\n========== STEP 5: STOP-WORD REMOVAL ==========\n");

    const char *negations[] = {"no", "not", "nor", "never", "neither"};
    int negation_count = 5;
    for (i = 0; i < sentence_count; i++)
    {
        struct word temp_tokens[MAX_WORDS];

        int old_count = sentence_token_count[i];

        for (j = 0; j < old_count; j++)
        {
            strcpy(temp_tokens[j].data, sentence_tokens[i][j]);

            temp_tokens[j].idx =
                (int)strlen(temp_tokens[j].data);
        }

        rear = old_count;

        int status = remove_stopwords(
        temp_tokens,
        "data/EN-Stopwords.txt",
        NULL,
        0
    );
        if (status == -1)
        {
            printf("Stop-word removal failed.\n");
            continue;
        }

        if (rear < 0 || rear > old_count)
        {
            printf("Invalid token count.\n");
            goto cleanup;
        }

        for (j = 0; j < rear; j++)
        {
            strcpy(
                sentence_tokens[i][j],
                temp_tokens[j].data
            );
        }

        for (j = rear; j < old_count; j++)
        {
            free(sentence_tokens[i][j]);
            sentence_tokens[i][j] = NULL;
        }

        sentence_token_count[i] = rear;

        printf("Sentence %d: ", i + 1);

        for (j = 0; j < rear; j++)
        {
            printf("%s", sentence_tokens[i][j]);

            if (j + 1 < rear)
                printf(" | ");
        }

        printf("\n");
    }
    printf("\nENTERING STEP 6...\n");
    fflush(stdout);
    /* STEP 6: WORD FREQUENCY */

    printf("\n========== STEP 6: WORD FREQUENCY ==========\n");

    char *combined_tokens[TOTAL_CAPACITY];

    int total_tokens = 0;

    for (i = 0; i < sentence_count; i++)
    {
        for (j = 0; j < sentence_token_count[i]; j++)
        {
            if (total_tokens < TOTAL_CAPACITY)
            {
                combined_tokens[total_tokens++] =
                    sentence_tokens[i][j];
            }
        }
    }

    TokenList all_tokens;

    all_tokens.tokens = combined_tokens;
    all_tokens.count = (size_t)total_tokens;
    all_tokens.capacity = (size_t)total_tokens;

    FreqList *freq_list = nlp_word_frequency(&all_tokens);

    if (freq_list == NULL)
    {
        printf("Frequency calculation failed.\n");
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

    /* STEP 7: STEMMING */

    printf("\n========== STEP 7: STEMMING ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        printf("Sentence %d: ", i + 1);

        for (j = 0; j < sentence_token_count[i]; j++)
        {
            nlp_stem_word(sentence_tokens[i][j]);

            printf("%s", sentence_tokens[i][j]);

            if (j + 1 < sentence_token_count[i])
                printf(" | ");
        }

        printf("\n");
    }

    /* STEP 8: BIGRAM GENERATION */

    printf("\n========== STEP 8: BIGRAM GENERATION (N = 2) ==========\n");

    for (i = 0; i < sentence_count; i++)
    {
        TokenList token_list;

        token_list.tokens = sentence_tokens[i];
        token_list.count = (size_t)sentence_token_count[i];
        token_list.capacity = token_list.count;

        printf("\nSentence %d:\n", i + 1);

        TokenList *bigrams = nlp_generate_ngrams(
            &token_list,
            2
        );

        if (bigrams == NULL)
        {
            printf("Not enough words to generate bigrams.\n");
            continue;
        }

        for (j = 0; j < (int)bigrams->count; j++)
        {
            printf("%s\n", bigrams->tokens[j]);
        }

        nlp_token_list_free(bigrams);
    }

    /* STEP 9: KEYWORD EXTRACTION */

    printf("\n========== STEP 9: KEYWORD EXTRACTION ==========\n");

    struct word *keyword_storage =
        calloc(TOTAL_CAPACITY, sizeof(struct word));

    struct word *keyword_input[TOTAL_CAPACITY];

    if (keyword_storage == NULL)
    {
        printf("Keyword memory allocation failed.\n");
        goto cleanup;
    }

    int keyword_count = 0;

    for (i = 0; i < sentence_count; i++)
    {
        for (j = 0; j < sentence_token_count[i]; j++)
        {
            if (keyword_count >= TOTAL_CAPACITY)
                break;

            strncpy(
                keyword_storage[keyword_count].data,
                sentence_tokens[i][j],
                sizeof(keyword_storage[keyword_count].data) - 1
            );

            keyword_storage[keyword_count].idx =
                (int)strlen(keyword_storage[keyword_count].data);

            keyword_input[keyword_count] =
                &keyword_storage[keyword_count];

            keyword_count++;
        }
    }

    KeywordList keywords = {NULL, 0, 0};

    int keyword_status = nlp_extract_keywords(
        keyword_input,
        (size_t)keyword_count,
        MAX_KEYWORDS,
        &keywords
    );

    if (keyword_status != 0)
    {
        printf("Keyword extraction failed: %d\n", keyword_status);
    }
    else
    {
        printf("\nTop Keywords:\n");

        for (i = 0; i < (int)keywords.count; i++)
        {
            printf("%-20s Frequency: %zu\n",
                   keywords.keywords[i].word,
                   keywords.keywords[i].frequency);
        }

        nlp_keyword_list_free(&keywords);
    }

    free(keyword_storage);

    printf("\n========================================\n");
    printf("       ALL 9 STEPS COMPLETED\n");
    printf("========================================\n");

cleanup:

    for (i = 0; i < MAX_SENTENCES; i++)
    {
        for (j = 0; j < sentence_token_count[i]; j++)
        {
            free(sentence_tokens[i][j]);
        }
    }

    return 0;
}