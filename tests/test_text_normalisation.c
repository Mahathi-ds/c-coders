
#include <stdio.h>
#include "project.h"

/* TEST CASES FOR TEXT NORMALIZATION */

void test_text_normalization()
{
    printf("\n========== TEXT NORMALIZATION TESTS ==========\n");

    /* Test 1: Normal input */
    char text1[] = "Hello!!! World 123 @ NLP";
    text_normalization(text1, DEFAULT_OPTIONS);
    printf("\nTest 1 - Normal input:\n%s\n", text1);
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

   
}

int main(void)
{
    test_text_normalization();
    return 0;
}
