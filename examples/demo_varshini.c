
#include <stdio.h>
#include "project.h"

/* TEST CASES FOR TEXT NORMALIZATION */

void test_text_normalization()
{
    printf("\n========== TEXT NORMALIZATION TESTS ==========\n");

    /* Test 1: Normal input */
    char text1[] = "Hello!!! World 123 @ NLP";
    text_normalization(text1, DEFAULT_OPTIONS);
    printf("Test 1 - Normal input:\n%s\n", text1);
    /* Expected: Hello World NLP */

    /* Test 2: Keep digits */
    char text2[] = "Hello 123 World 456";
    text_normalization(text2, DIGIT_REMOVAL_FALSE);
    printf("\nTest 2 - Keep digits:\n%s\n", text2);
    /* Expected: Hello 123 World 456 */

    /* Test 3: Keep punctuation */
    char text3[] = "Hello!!! World? NLP.";
    text_normalization(text3, PUNCTUATION_REMOVAL_FALSE);
    printf("\nTest 3 - Keep punctuation:\n%s\n", text3);
    /* Expected: Hello!!! World? NLP. */

    /* Test 4: Empty input */
    char text4[] = "";
    text_normalization(text4, DEFAULT_OPTIONS);
    printf("\nTest 4 - Empty input:\n'%s'\n", text4);
    /* Expected: empty string */

    /* Test 5: Only spaces */
    char text5[] = "       ";
    text_normalization(text5, DEFAULT_OPTIONS);
    printf("\nTest 5 - Only spaces:\n'%s'\n", text5);
    /* Expected: empty string */

    /* Test 6: Leading, trailing and extra spaces */
    char text6[] = "   Hello     World   ";
    text_normalization(text6, DEFAULT_OPTIONS);
    printf("\nTest 6 - Extra spaces:\n'%s'\n", text6);
    /* Expected: Hello World */

    /* Test 7: Single character */
    char text7[] = "A";
    text_normalization(text7, DEFAULT_OPTIONS);
    printf("\nTest 7 - Single character:\n%s\n", text7);
    /* Expected: A */

    /* Test 8: All removable characters */
    char text8[] = "123!!!@@@";
    text_normalization(text8, DEFAULT_OPTIONS);
    printf("\nTest 8 - Only digits and punctuation:\n'%s'\n", text8);
    /* Expected: empty string */

    /* Test 9: Non-printable character */
    char text9[] = {'A', '\1', 'B', '\0'};
    text_normalization(text9, DEFAULT_OPTIONS);
    printf("\nTest 9 - Non-printable character:\n%s\n", text9);
    /* Expected: AB */

    /* Test 10: NULL input */
    printf("\nTest 10 - NULL input:\n");
    text_normalization(NULL, DEFAULT_OPTIONS);
    printf("Function returned safely.\n");
    /* Expected: no crash */

    /* Test 11: Large input */
    char large_text[5000];
    int i;
    int j = 0;
    int length = 0;

    for (i = 0; i < 1200; i++)
    {
        large_text[j] = 'A';
        j++;
        large_text[j] = '1';
        j++;
        large_text[j] = '!';
        j++;
        large_text[j] = ' ';
        j++;
    }
    large_text[j] = '\0';

    text_normalization(large_text, DEFAULT_OPTIONS);

    while (large_text[length] != '\0')
    {
        length++;
    }

    printf("\nTest 11 - Large input:\n");
    printf("Normalization completed.\n");
    printf("Output length: %d\n", length);
    /* Expected length: 2399 */

    /* Test 12: Custom option values */
    char text12[] = "Hello 123!!! World";
    Option no_removal = {0, 0, 0, 0};

    text_normalization(text12, no_removal);
    printf("\nTest 12 - All removal flags disabled:\n%s\n", text12);
    /* Expected: Hello 123!!! World */
}


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
    /* Expected: 0 */

    /* Test 5: Negative maximum sentences */
    count = sentence_split(text4, sentences, -1);

    printf("\nTest 5 - Negative maximum sentences:\n");
    printf("Sentence count: %d\n", count);
    /* Expected: 0 */

    /* Test 6: No punctuation */
    char text6[] = "This is a sentence without punctuation";

    count = sentence_split(text6, sentences, 10);

    printf("\nTest 6 - No punctuation:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: %s\n", i + 1, sentences[i]);
    }
    /* Expected count: 1 */

    /* Test 7: Single character */
    char text7[] = "A.";

    count = sentence_split(text7, sentences, 10);

    printf("\nTest 7 - Single character:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: %s\n", i + 1, sentences[i]);
    }
    /* Expected count: 1 */

    /* Test 8: Different sentence-ending punctuation */
    char text8[] = "Hi! Really? Yes.";

    count = sentence_split(text8, sentences, 10);

    printf("\nTest 8 - Different punctuation:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: %s\n", i + 1, sentences[i]);
    }
    /* Expected count: 3 */

    /* Test 9: Maximum sentence limit */
    char text9[] = "One. Two. Three.";

    count = sentence_split(text9, sentences, 1);

    printf("\nTest 9 - Maximum sentence limit is 1:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: %s\n", i + 1, sentences[i]);
    }
    /* Expected count: 1 */

    /* Test 10: Leading and multiple spaces */
    char text10[] = "   Hello!     Welcome.   Bye?";

    count = sentence_split(text10, sentences, 10);

    printf("\nTest 10 - Extra spaces:\n");
    printf("Sentence count: %d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%d: '%s'\n", i + 1, sentences[i]);
    }
    /* Expected count: 3 */

    /* Test 11: Large input with many short sentences */
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

    printf("\nTest 11 - Large input:\n");
    printf("Sentence count: %d\n", count);
    /* Expected count: 10 because max_sentences is 10 */

    /* Test 12: Sentence near buffer capacity */
    char long_text[500];
    char long_sentences[1][500];

    for (i = 0; i < 499; i++)
    {
        long_text[i] = 'A';
    }
    long_text[499] = '\0';

    count = sentence_split(long_text, long_sentences, 1);

    printf("\nTest 12 - Sentence of 499 characters:\n");
    printf("Sentence count: %d\n", count);
    /* Expected count: 1 */
}


int main()
{
    test_text_normalization();
    test_sentence_split();

    return 0;
}
