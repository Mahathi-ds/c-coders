#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "project.h"



int sim_find_word(FreqList *list, const char *word)
{
    int i;

    for (i = 0; i < list->count; i++)
    {
        if (strcmp(list->frequencies[i].word, word) == 0)
        {
            return i;
        }
    }

    return -1;
}



static int sim_add_word(FreqList *list, const char *word)
{
    int index;
    char *new_word;

    index = sim_find_word(list, word);

    if (index != -1)
    {
        list->frequencies[index].count++;
        return 1;
    }

    if (list->count == list->capacity)
    {
        size_t new_capacity;

        new_capacity = list->capacity * 2;

        if (new_capacity == 0)
        {
            new_capacity = 10;
        }

        WordFreq *temp;

        temp = realloc(
            list->frequencies,
            new_capacity * sizeof(WordFreq)
        );

        if (temp == NULL)
        {
            return 0;
        }

        list->frequencies = temp;
        list->capacity = new_capacity;
    }

    new_word = malloc(strlen(word) + 1);

    if (new_word == NULL)
    {
        return 0;
    }

    strcpy(new_word, word);

    list->frequencies[list->count].word = new_word;
    list->frequencies[list->count].count = 1;

    list->count++;

    return 1;
}



int sim_build_frequency_list(const char *filename, FreqList *list)
{
    FILE *fp;
    int ch;
    int length;
    char word[1000];

    list->frequencies = NULL;
    list->count = 0;
    list->capacity = 0;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        return 0;
    }

    length = 0;

    while ((ch = fgetc(fp)) != EOF)
    {
        if (isalnum((unsigned char)ch))
        {
            if (length < 999)
            {
                word[length] = (char)tolower((unsigned char)ch);
                length++;
            }
        }
        else
        {
            if (length > 0)
            {
                word[length] = '\0';

                if (!sim_add_word(list, word))
                {
                    fclose(fp);
                    sim_free_frequency_list(list);
                    return 0;
                }

                length = 0;
            }
        }
    }

    if (length > 0)
    {
        word[length] = '\0';

        if (!sim_add_word(list, word))
        {
            fclose(fp);
            sim_free_frequency_list(list);
            return 0;
        }
    }

    fclose(fp);

    return 1;
}


/* Free the frequency list */

void sim_free_frequency_list(FreqList *list)
{
    size_t i;

    for (i = 0; i < list->count; i++)
    {
        free(list->frequencies[i].word);
    }

    free(list->frequencies);

    list->frequencies = NULL;
    list->count = 0;
    list->capacity = 0;
}