
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

void nlp_stem_word(char *word);

static int passed = 0;
static int failed = 0;

static void test_stem(const char *name,
                      const char *input,
                      const char *expected)
{
    char word[256];

    snprintf(word, sizeof(word), "%s", input);
    nlp_stem_word(word);

    if (strcmp(word, expected) == 0) {
        printf("[PASS] %-30s %s -> %s\n",
               name, input, word);
        passed++;
    } else {
        printf("[FAIL] %s\n", name);
        printf("       Input:    %s\n", input);
        printf("       Expected: %s\n", expected);
        printf("       Actual:   %s\n", word);
        failed++;
    }
}

int main(void)
{
    printf("========== PORTER STEMMER TESTS ==========\n\n");

    /* 1. Basic words */
    test_stem("Computer unchanged", "computer", "computer");
    test_stem("Yes unchanged", "yes", "yes");
    test_stem("Cat unchanged", "cat", "cat");
    test_stem("Dog unchanged", "dog", "dog");
    test_stem("Book unchanged", "book", "book");

    /* 2. Plurals and Step 1a */
    test_stem("Caresses", "caresses", "caress");
    test_stem("Ponies", "ponies", "poni");
    test_stem("Ties", "ties", "ti");
    test_stem("Cats", "cats", "cat");
    test_stem("Running", "running", "run");

    /* 3. Past tense and -ing */
    test_stem("Agreed", "agreed", "agre");
    test_stem("Disabled", "disabled", "disabl");
    test_stem("Matting", "matting", "mat");
    test_stem("Mating", "mating", "mate");
    test_stem("Meeting", "meeting", "meet");
    test_stem("Milling", "milling", "mill");
    test_stem("Messing", "messing", "mess");

    /* 4. Step 2 suffixes */
    test_stem("Relational", "relational", "relat");
    test_stem("Conditional", "conditional", "condit");
    test_stem("Rational", "rational", "ration");
    test_stem("Vietnamization",
              "vietnamization", "vietnam");
    test_stem("Digitizer", "digitizer", "digit");

    /* 5. Step 3 suffixes */
    test_stem("Triplicate", "triplicate", "triplic");
    test_stem("Formalize", "formalize", "formal");
    test_stem("Electrical", "electrical", "electr");
    test_stem("Hopeful", "hopeful", "hope");
    test_stem("Goodness", "goodness", "good");

    /* 6. Step 4 suffixes */
    test_stem("Revival", "revival", "reviv");
    test_stem("Allowance", "allowance", "allow");
    test_stem("Inference", "inference", "infer");
    test_stem("Airliner", "airliner", "airlin");
    test_stem("Gyroscopic", "gyroscopic", "gyroscop");

    /* 7. Boundary cases */
    test_stem("Empty string", "", "");
    test_stem("One character", "a", "a");
    test_stem("Two characters", "to", "to");
    test_stem("Short word yes", "yes", "yes");

    /* 8. Uppercase input
       This implementation converts ASCII to lowercase. */
    test_stem("Uppercase PLAYING", "PLAYING", "play");
    test_stem("Mixed case COMPUTER", "Computer", "computer");

    /* 9. Long input */
    char long_word[256];
    char expected_long[256];

    memset(long_word, 'a', 200);
    long_word[200] = '\0';

    memset(expected_long, 'a', 200);
    expected_long[200] = '\0';

    test_stem("Long word", long_word, expected_long);

    /* 10. NULL input must not crash */
    nlp_stem_word(NULL);
    printf("[PASS] NULL input does not crash\n");
    passed++;

    printf("\n========== TEST SUMMARY ==========\n");
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
    printf("Total:  %d\n", passed + failed);

    if (failed == 0)
        printf("\nAll tests passed!\n");
    else
        printf("\nSome tests failed. Check the actual results above.\n");

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}