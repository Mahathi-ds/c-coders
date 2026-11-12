#include<stdio.h>
#include "project.h"
#include<stdlib.h>
#include<string.h>


int stop_words(const char *filename, char stopwords[][100])
{
    if (filename == NULL || stopwords == NULL)
        return -1;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror("Error opening stopwords file");
        return -1;
    }

    int count = 0;

    while (count < 1000 &&
           fscanf(fp, "%99s", stopwords[count]) == 1)
    {
        count++;
    }

    fclose(fp);
    return count;
}

int remove_stopwords(struct word *tokens,const char *filename,const char **negations,int negation_count){
    if (tokens == NULL || rear == 0 || filename == NULL)
        return -1;

    if (negation_count < 0 || (negation_count > 0 && negations == NULL))
        return -1;

    char stopwords[1000][100];

    int count = stop_words(filename, stopwords);

    if (count == -1)
        return -1;

    int i = 0;

    while (i < rear)
    {
        make_lower(tokens[i].data);

        int found = 0;

        for (int j = 0; j < count; j++)
        {
            if (strcmp(tokens[i].data, stopwords[j]) == 0)
            {
                int is_negation = 0;

                for (int k = 0; k < negation_count; k++)
                {
                    if (strcmp(tokens[i].data, negations[k]) == 0)
                    {
                        is_negation = 1;
                        break;
                    }
                }

                if (is_negation)
                    break;

                for (int k = i; k < rear - 1; k++)
                    tokens[k] = tokens[k + 1];
                rear--;
                found = 1;
                break;
            }
        }

        if (found == 0)
            i++;
    }

    return 0;
}