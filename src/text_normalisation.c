
#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include "project.h"

void text_normalization(char *text, Option opt)
{
    int digit_removal = 1;
    int punctuation_removal = 1;
    int special_character_removal = 1;
    int extra_space_removal = 1;

    int i = 0;
    int j = 0;
    int inside_space = 0;
    char ch;

    if (text == NULL)
    {
        return;
    }

    digit_removal = opt.digit_removal;
    punctuation_removal = opt.punctuation_removal;
    special_character_removal = opt.special_character_removal;
    extra_space_removal = opt.extra_space_removal;

    while (text[i] != '\0')
    {
        ch = text[i];

        /* Handle whitespace */
        if (isspace((unsigned char)ch))
        {
            if (extra_space_removal)
            {
                if (j > 0 && inside_space == 0)
                {
                    text[j] = ' ';
                    j++;
                    inside_space = 1;
                }
            }
            else
            {
                text[j] = ch;
                j++;
            }

            i++;
            continue;
        }

        /* Remove digits */
        if (digit_removal && isdigit((unsigned char)ch))
        {
            i++;
            continue;
        }

        /* Remove punctuation */
        if (punctuation_removal && ispunct((unsigned char)ch))
        {
            i++;
            continue;
        }

        /* Remove non-printable characters */
        if (special_character_removal &&
            !isprint((unsigned char)ch))
        {
            i++;
            continue;
        }

        /* Keep the character */
        text[j] = ch;
        j++;
        inside_space = 0;

        i++;
    }

    /* Remove trailing space */
    if (extra_space_removal && j > 0 && text[j - 1] == ' ')
    {
        j--;
    }

    text[j] = '\0';
}
