
#include <stdio.h>
#include "project.h"


void test_text_normalization()
{
    printf("\n========== TEXT NORMALIZATION TESTS ==========\n");

    char text1[] = "Hello!!! World 123 @ NLP";

    printf("\nTest 1 - Normal input\n");
    printf("Input : %s\n", text1);
    text_normalization(text1, DEFAULT_OPTIONS);
    printf("Output: %s\n", text1);

    char text2[] = "Hello!! 123  World 456";

    printf("\nTest 2 - Keep digits\n");
    printf("Input : %s\n", text2);
    text_normalization(text2, DIGIT_REMOVAL_FALSE);
    printf("Output: %s\n", text2);

    char text3[] = "Hello!!! World? NLP.";

    printf("\nTest 3 - Keep punctuation\n");
    printf("Input : %s\n", text3);
    text_normalization(text3, PUNCTUATION_REMOVAL_FALSE);
    printf("Output: %s\n", text3);

    char text4[] = "";

    printf("\nTest 4 - Empty input\n");
    printf("Input : %s\n", text4);
    text_normalization(text4, DEFAULT_OPTIONS);
    printf("Output: %s\n", text4);

    char text5[] = "       ";

    printf("\nTest 5 - Only spaces\n");
    printf("Input : %s\n", text5);
    text_normalization(text5, DEFAULT_OPTIONS);
    printf("Output: %s\n", text5);

    char text6[] = "   Hello     World   ";

    printf("\nTest 6 - Extra spaces\n");
    printf("Input : %s\n", text6);
    text_normalization(text6, DEFAULT_OPTIONS);
    printf("Output: %s\n", text6);

    char text7[] = "A";

    printf("\nTest 7 - Single character\n");
    printf("Input : %s\n", text7);
    text_normalization(text7, DEFAULT_OPTIONS);
    printf("Output: %s\n", text7);

    char text8[] = "123!!!@@@";

    printf("\nTest 8 - Only digits and punctuation\n");
    printf("Input : %s\n", text8);
    text_normalization(text8, DEFAULT_OPTIONS);
    printf("Output: %s\n", text8);

    printf("\nTest 9 - NULL input\n");
    printf("Input : NULL\n");
    text_normalization(NULL, DEFAULT_OPTIONS);
    printf("Output: Function returned safely\n");

    char large_text[5000];
    int i, j = 0;
    int length = 0;

    for (i = 0; i < 1200; i++)
    {
        large_text[j++] = 'A';
        large_text[j++] = '1';
        large_text[j++] = '!';
        large_text[j++] = ' ';
    }

    large_text[j] = '\0';

    printf("\nTest 10 - Large input\n");
    printf("Input : 1200 repetitions of \"A1! \"\n");

    text_normalization(large_text, DEFAULT_OPTIONS);

    while (large_text[length] != '\0')
    {
        length++;
    }

    printf("Output: %.50s...\n", large_text);
    printf("Output length: %d\n", length);
}

int main(void)
{
    test_text_normalization();
    return 0;
}
