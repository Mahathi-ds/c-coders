#include <stdio.h>
#include "project.h"

int main()
{
    char text1[] = "Hello!!! World 123 @ NLP";
    char text2[] = "Hello!!! World 123 @ NLP";
    char text3[] = "Hello!!! World 123 @ NLP";

    text_normalization(text1, DEFAULT_OPTIONS);
    text_normalization(text2, DIGIT_REMOVAL_FALSE);
    text_normalization(text3, PUNCTUATION_REMOVAL_FALSE);

    printf("Default: %s\n", text1);
    printf("Keep digits: %s\n", text2);
    printf("Keep punctuation: %s\n", text3);
	char paragraph[] = "Hello world! Welcome to NLP. How are you?";
    char sentences[10][500];

    int count = sentence_split(paragraph, sentences, 10);

    printf("\nSENTENCE SPLITTING\n");
    printf("------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("Sentence %d: %s\n", i + 1, sentences[i]);
    }
    return 0;
}