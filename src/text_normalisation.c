
#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include "project.h"

void text_normalization(char *text, Option opt)
{
    int digit_removal = opt.digit_removal;
    int punctuation_removal = opt.punctuation_removal;
    int special_character_removal = opt.special_character_removal;

    int i = 0;
    int j = 0;
    int inside_space = 0;
    char ch;

    if (text == NULL)
    {
        return;
    }

    while (text[i] != '\0')
    {
        ch = text[i];

        /* Always remove extra spaces */
        if (isspace((unsigned char)ch))
        {
            if (j > 0 && inside_space == 0)
            {
                text[j] = ' ';
                j++;
                inside_space = 1;
            }

            i++;
            continue;
        }

        if (digit_removal && isdigit((unsigned char)ch))
        {
            i++;
            continue;
        }
        if (punctuation_removal && ispunct((unsigned char)ch))
        {
            i++;
            continue;
        }
        if (special_character_removal &&
            !isprint((unsigned char)ch))
        {
            i++;
            continue;
        }

        text[j] = ch;
        j++;
        inside_space = 0;

        i++;
    }
    if (j > 0 && text[j - 1] == ' ')
    {
        j--;
    }

    text[j] = '\0';
}