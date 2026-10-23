#include <stdio.h>
#include <string.h>
#include "project.h"

/* TEST CASES FOR SENTENCE SPLITTING */

void test_sentence_split()
{
    printf("\n========== SENTENCE SPLITTING TESTS ==========\n");

    char sentences[10][500];
    int count;
    int i;

    /* Test 1: Normal input */
    char text1[] = "Hello world! Welcome to NLP. How are you?";

    count = sentence_split(text1, sentences, 10);

    printf("\nTest 1 - Normal input:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: %s\n", i + 1, sentences[i]);
    }
    /* Expected count: 3 */

    /* Test 2: Empty input */
    char text2[] = "";

    count = sentence_split(text2, sentences, 10);

    printf("\nTest 2 - Empty input:\n");
    printf("Sentence count: %d\n", count);
    /* Expected: 0 */

    /* Test 3: NULL input */
    count = sentence_split(NULL, sentences, 10);

    printf("\nTest 3 - NULL input:\n");
    printf("Sentence count: %d\n", count);
    /* Expected: 0 */

    /* Test 4: Zero maximum sentences */
    char text4[] = "Hello. World.";

    count = sentence_split(text4, sentences, 0);

    printf("\nTest 4 - Zero maximum sentences:\n");
    printf("Sentence count: %d\n", count);

    count = sentence_split(text4, sentences, -3);

    printf("\nTest 5 - Negative maximum sentences:\n");
    printf("Sentence count: %d\n", count);

    char text6[] = "This is a sentence without punctuation";

    count = sentence_split(text6, sentences, 10);

    printf("\nTest 6 - No punctuation:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: %s\n", i + 1, sentences[i]);
    }

    char text7[] = "A.";

    count = sentence_split(text7, sentences, 10);

    printf("\nTest 7 - Single character:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: %s\n", i + 1, sentences[i]);
    }

    char text8[] = "Hi! Really? Yes.";

    count = sentence_split(text8, sentences, 10);

    printf("\nTest 8 - Different punctuation:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: %s\n", i + 1, sentences[i]);
    }



    char text9[] = "   Hello!     Welcome.   Bye?";

    count = sentence_split(text9, sentences, 10);

    printf("\nTest 9 - Extra spaces:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: '%s'\n", i + 1, sentences[i]);
    }
    
    char large_text[5000];
    int j = 0;

    for (i = 0; i < 1000; i++)
    {
        large_text[j] = 'A';
        j++;
        large_text[j] = '.';
        j++;
        large_text[j] = ' ';
        j++;
    }
    large_text[j] = '\0';

    count = sentence_split(large_text, sentences, 10);

    printf("\nTest 10 - Large input:\n");
    printf("Sentence count: %d\n", count);

    char long_text[500];
    char long_sentences[1][500];

    for (i = 0; i < 499; i++)
    {
        long_text[i] = 'A';
    }
    long_text[499] = '\0';

    count = sentence_split(long_text, long_sentences, 1);

    printf("\nTest 11 - Sentence of 499 characters:\n");
    printf("Sentence count: %d\n", count);

    char text13[] = "Hello! Welcome to NLP. How are you?";
    char default_sentences[sizeof(text13)][500];

    count = sentence_split(text13, default_sentences, DEFAULT);

    printf("\nTest 12 - DEFAULT maximum sentences:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: %s\n", i + 1, default_sentences[i]);
    }
    /* Expected count: 3 because DEFAULT uses the length of the input text */
}

int main(void)
{
    test_sentence_split();
    return 0;
}