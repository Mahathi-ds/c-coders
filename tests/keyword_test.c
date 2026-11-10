
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "project.h"


#define MAX_TOKENS 1000

int rear = 0;

/* Stop words used only by this test file */
static const char *stop_words[] = {
    "a", "an", "the",
    "is", "am", "are", "was", "were", "be", "been", "being",
    "and", "or", "but", "if", "because",
    "of", "at", "by", "for", "with", "about",
    "against", "between", "into", "through",
    "during", "before", "after", "above", "below",
    "to", "from", "in", "on", "off", "out",
    "over", "under",
    "it", "its", "this", "that", "these", "those",
    "he", "she", "they", "them", "we", "you", "i",
    "me", "my", "your", "his", "her", "our", "their",
    "as", "what", "which", "who", "whom",
    "when", "where", "why", "how",
    "can", "could", "will", "would", "shall", "should",
    "may", "might", "must",
    "do", "does", "did", "doing",
    "have", "has", "had",
    "not", "no", "so", "than", "then",
    "too", "very", "just",
    "also", "only", "own", "same",
    "such", "up", "down"
};

#define STOP_WORD_COUNT \
    (sizeof(stop_words) / sizeof(stop_words[0]))

/* Check whether a word is a stop word */
static int is_stop_word(const char *word)
{
    for (size_t i = 0; i < STOP_WORD_COUNT; i++) {
        if (strcmp(word, stop_words[i]) == 0)
            return 1;
    }

    return 0;
}

/* Remove stop words from already-tokenized input */
static int remove_stopwords(struct word *tokens[])
{
    if (tokens == NULL)
        return -1;

    int write_index = 0;

    for (int read_index = 0; read_index < rear; read_index++) {

        if (tokens[read_index] == NULL)
            continue;

        if (is_stop_word(tokens[read_index]->data)) {
            free(tokens[read_index]);
            tokens[read_index] = NULL;
        }
        else {
            tokens[write_index] = tokens[read_index];
            tokens[write_index]->idx = write_index;
            write_index++;
        }
    }

    /* Update token count after removal */
    rear = write_index;

    /* Clear unused positions */
    for (int i = rear; i < MAX_TOKENS; i++)
        tokens[i] = NULL;

    return 0;
}

/* Tokenize the input sentence */
static int tokenize_sentence(
    const char *sentence,
    struct word *tokens[]
)
{
    rear = 0;

    char word[1000];
    int j = 0;

    for (int i = 0; ; i++) {
        unsigned char ch = (unsigned char)sentence[i];

        if (isalnum(ch)) {
            if (j < 999)
                word[j++] = (char)tolower(ch);
        }
        else {
            if (j > 0) {
                word[j] = '\0';

                if (rear >= MAX_TOKENS)
                    return -1;

                tokens[rear] = malloc(sizeof(struct word));

                if (tokens[rear] == NULL)
                    return -1;

                strcpy(tokens[rear]->data, word);
                tokens[rear]->idx = rear;

                rear++;
                j = 0;
            }

            if (sentence[i] == '\0')
                break;
        }
    }

    return 0;
}

/* Release remaining tokens */
static void free_tokens(struct word *tokens[])
{
    for (int i = 0; i < rear; i++) {
        free(tokens[i]);
        tokens[i] = NULL;
    }

    rear = 0;
}

/* Run an individual test case */
static void run_test(int test_number, const char *sentence)
{
    struct word *tokens[MAX_TOKENS] = {0};

    printf("\n====================================\n");
    printf("TEST CASE %d\n", test_number);
    printf("====================================\n");

    printf("Input: %s\n", sentence);

    /* Step 1: Tokenization */
    if (tokenize_sentence(sentence, tokens) != 0) {
        printf("Tokenization failed\n");
        free_tokens(tokens);
        return;
    }

    printf("\nTokens before stop-word removal:\n");

    for (int i = 0; i < rear; i++)
        printf("%s ", tokens[i]->data);

    /* Step 2: Stop-word removal */
    if (remove_stopwords(tokens) != 0) {
        printf("\nStop-word removal failed\n");
        free_tokens(tokens);
        return;
    }

    printf("\n\nTokens after stop-word removal:\n");

    for (int i = 0; i < rear; i++)
        printf("%s ", tokens[i]->data);

    /* Step 3: Keyword extraction */
    KeywordList result = {0};

    int status = nlp_extract_keywords(
        tokens,
        (size_t)rear,
        3,
        &result
    );

    printf("\n\nExtracted Keywords:\n");

    if (status == 0) {

        if (result.count == 0)
            printf("No keywords found.\n");

        for (size_t i = 0; i < result.count; i++) {
            printf("%-20s Frequency: %zu\n",
                   result.keywords[i].word,
                   result.keywords[i].frequency);
        }
    }
    else {
        printf("Keyword extraction failed. Error: %d\n", status);
    }

    /* Step 4: Free allocated memory */
    nlp_keyword_list_free(&result);
    free_tokens(tokens);
}

int main(void)
{
    const char *test_cases[] = {

        "The cat is sitting on the mat. The cat is cute.",

        "Machine learning is useful. Machine learning is powerful.",

        "Data DATA data science Science",

        "The is are was were and but",

        "",

        "Hello, hello! World. World?",

        "123 machine 456 learning machine",

        "Artificial intelligence is transforming the world. Artificial intelligence helps people solve problems.",

        "Apple banana mango orange",

        "Python is easy. Python is powerful. Python is popular.",

        "Electricity",

        "computer computer computer computer",

        "Data, science! Data? Science.",

        "Technology improves life. Technology improves education. Technology improves communication.",

        "AI@2026 is amazing!!! AI is useful.",

        "Machine learning is a powerful technology.Machine learning helps computers learn.Deep learning is a part of machine learning.Computers use machine learning for predictions.Deep learning improves predictions."
    };

    size_t total_tests =
        sizeof(test_cases) / sizeof(test_cases[0]);

    printf("KEYWORD EXTRACTION TESTING\n");

    for (size_t i = 0; i < total_tests; i++) {
        run_test((int)i + 1, test_cases[i]);
    }

    printf("\n====================================\n");
    printf("ALL TEST CASES COMPLETED\n");
    printf("====================================\n");

    return 0;
}
