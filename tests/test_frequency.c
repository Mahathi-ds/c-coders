
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

FreqList *nlp_word_frequency(TokenList *tokens);

static void free_freq_list(FreqList *list)
{
    if (list == NULL)
        return;

    free(list->frequencies);
    free(list);
}

static int get_actual_frequency(FreqList *list, const char *word)
{
    if (list == NULL)
        return 0;

    for (size_t i = 0; i < list->count; i++) {
        if (strcmp(list->frequencies[i].word, word) == 0)
            return list->frequencies[i].count;
    }

    return 0;
}

static void run_test(
    const char *title,
    char **words,
    size_t word_count,
    const char **expected_words,
    const int *expected_counts,
    size_t expected_unique_count)
{
    TokenList input = {words, word_count, word_count};

    printf("\n========================================\n");
    printf("TEST: %s\n", title);
    printf("========================================\n");

    printf("INPUT WORDS:\n");
    for (size_t i = 0; i < word_count; i++)
        printf("%s ", words[i]);
    printf("\n");

    FreqList *actual = nlp_word_frequency(&input);

    if (actual == NULL) {
        printf("\nACTUAL OUTPUT: NULL\n");
        printf("Unable to calculate frequencies.\n");
        return;
    }

    printf("\nEXPECTED FREQUENCIES:\n");
    if (expected_unique_count == 0) {
        printf("(empty)\n");
    } else {
        for (size_t i = 0; i < expected_unique_count; i++)
            printf("%-15s : %d\n",
                   expected_words[i], expected_counts[i]);
    }

    printf("\nACTUAL FREQUENCIES:\n");
    if (actual->count == 0) {
        printf("(empty)\n");
    } else {
        for (size_t i = 0; i < actual->count; i++)
            printf("%-15s : %d\n",
                   actual->frequencies[i].word,
                   actual->frequencies[i].count);
    }

    printf("\nCOMPARISON:\n");

    int correct = (actual->count == expected_unique_count);

    for (size_t i = 0; i < expected_unique_count; i++) {
        int found_count = get_actual_frequency(
            actual, expected_words[i]);

        printf("%-15s Expected = %d, Actual = %d\n",
               expected_words[i], expected_counts[i], found_count);

        if (found_count != expected_counts[i])
            correct = 0;
    }

    /* Also ensure no unexpected unique words were returned. */
    for (size_t i = 0; i < actual->count; i++) {
        int expected_found = 0;

        for (size_t j = 0; j < expected_unique_count; j++) {
            if (strcmp(actual->frequencies[i].word,
                       expected_words[j]) == 0) {
                expected_found = 1;
                break;
            }
        }

        if (!expected_found) {
            printf("Unexpected word: %s, Actual = %d\n",
                   actual->frequencies[i].word,
                   actual->frequencies[i].count);
            correct = 0;
        }
    }

    printf("\nFINAL RESULT: %s\n",
           correct ? "All frequencies match" : "Frequency mismatch");

    free_freq_list(actual);
}

int main(void)
{
    /* Test 1: Normal input */
    char *words1[] = {
        "apple", "banana", "apple", "mango",
        "banana", "apple", "orange", "mango", "banana"
    };
    const char *expected1[] = {"apple", "banana", "mango", "orange"};
    const int counts1[] = {3, 3, 2, 1};

    run_test("Normal input",
             words1, 9, expected1, counts1, 4);

    /* Test 2: All words repeated */
    char *words2[] = {"hello", "hello", "hello", "hello"};
    const char *expected2[] = {"hello"};
    const int counts2[] = {4};

    run_test("Repeated words",
             words2, 4, expected2, counts2, 1);

    /* Test 3: All unique words */
    char *words3[] = {"red", "blue", "green", "yellow"};
    const char *expected3[] = {"red", "blue", "green", "yellow"};
    const int counts3[] = {1, 1, 1, 1};

    run_test("All unique words",
             words3, 4, expected3, counts3, 4);

    /* Test 4: Single token */
    char *words4[] = {"computer"};
    const char *expected4[] = {"computer"};
    const int counts4[] = {1};

    run_test("Single word",
             words4, 1, expected4, counts4, 1);

    /* Test 5: Empty input */
    run_test("Empty input",
             NULL, 0, NULL, NULL, 0);

    /* Test 6: Case-sensitive words */
    char *words6[] = {"Apple", "apple", "Apple", "APPLE"};
    const char *expected6[] = {"Apple", "apple", "APPLE"};
    const int counts6[] = {2, 1, 1};

    run_test("Case sensitivity",
             words6, 4, expected6, counts6, 3);

    /* Test 7: Punctuation */
    char *words7[] = {"hello", "hello,", "hello", "world!"};
    const char *expected7[] = {"hello", "hello,", "world!"};
    const int counts7[] = {2, 1, 1};

    run_test("Punctuation",
             words7, 4, expected7, counts7, 3);

    /* Test 8: Large input */
    const size_t total = 1000;
    char **large_words = malloc(total * sizeof(char *));

    if (large_words == NULL) {
        fprintf(stderr, "Unable to allocate large input.\n");
        return EXIT_FAILURE;
    }

    size_t allocated = 0;

    for (size_t i = 0; i < total; i++) {
        large_words[i] = malloc(32);

        if (large_words[i] == NULL)
            break;

        snprintf(large_words[i], 32, "word%zu", i % 10);
        allocated++;
    }

    if (allocated == total) {
        const char *expected_large[] = {
            "word0", "word1", "word2", "word3", "word4",
            "word5", "word6", "word7", "word8", "word9"
        };
        const int counts_large[] = {
            100, 100, 100, 100, 100,
            100, 100, 100, 100, 100
        };

        run_test("Large input (1000 words)",
                 large_words, total,
                 expected_large, counts_large, 10);
    } else {
        printf("Large input allocation failed after %zu words.\n",
               allocated);
    }

    for (size_t i = 0; i < allocated; i++)
        free(large_words[i]);

    free(large_words);

    /* Test 9: Invalid NULL input */
    printf("\n========================================\n");
    printf("TEST: NULL input\n");
    printf("========================================\n");

    FreqList *null_result = nlp_word_frequency(NULL);

    printf("Expected output: NULL\n");
    printf("Actual output:   %s\n",
           null_result == NULL ? "NULL" : "Non-NULL");

    if (null_result != NULL)
        free_freq_list(null_result);

    printf("\n========== TESTING FINISHED ==========\n");

    return EXIT_SUCCESS;
}