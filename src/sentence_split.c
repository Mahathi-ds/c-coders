#include <ctype.h>
#include <string.h>
#include "project.h"

int sentence_split(char *text, char sentences[][SENTENCE_SIZE],
                   int max_sentences)
{
    int i = 0;
    int j = 0;
    int sentence_count = 0;
    char ch;

    if (text == NULL || sentences == NULL)
    {
        return 0;
    }

    if (max_sentences == DEFAULT)
    {
        max_sentences = (int)strlen(text);
    }

    if (max_sentences <= 0)
    {
        return 0;
    }

    while (text[i] != '\0' && sentence_count < max_sentences)
    {
        ch = text[i];

        if (j < SENTENCE_SIZE - 1)
        {
            sentences[sentence_count][j] = ch;
            j++;
        }

        if (ch == '.' || ch == '!' || ch == '?')
        {
            sentences[sentence_count][j] = '\0';
            sentence_count++;
            j = 0;

            i++;

            while (text[i] != '\0' &&
                   isspace((unsigned char)text[i]))
            {
                i++;
            }

            continue;
        }

        i++;
    }

    if (j > 0 && sentence_count < max_sentences)
    {
        sentences[sentence_count][j] = '\0';
        sentence_count++;
    }

    return sentence_count;
}