
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "project.h"



GloveWord *glove_lookup(GloveModel *model, const char *word)
{
    unsigned long position;
    int i;
    int index;

    if (model == NULL || word == NULL ||
        model->hash_table == NULL || model->words == NULL)
    {
        return NULL;
    }

    position = glove_hash(word);

    for (i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (model->hash_table[position] == 0)
        {
            return NULL;
        }

        index = model->hash_table[position] - 1;

        if (strcmp(model->words[index].word, word) == 0)
        {
            return &model->words[index];
        }

        position = (position + 1) % HASH_TABLE_SIZE;
    }

    return NULL;
}



float glove_word_similarity(GloveModel *model,
                            const char *word1,
                            const char *word2)
{
    GloveWord *w1;
    GloveWord *w2;

    float dot_product;
    float magnitude1;
    float magnitude2;
    float similarity;

    int i;

    if (model == NULL || word1 == NULL || word2 == NULL)
    {
        return -1.0f;
    }

    w1 = glove_lookup(model, word1);
    w2 = glove_lookup(model, word2);

    if (w1 == NULL || w2 == NULL)
    {
        return -1.0f;
    }

    dot_product = 0.0f;
    magnitude1 = 0.0f;
    magnitude2 = 0.0f;

    for (i = 0; i < GLOVE_DIM; i++)
    {
        dot_product += w1->vector[i] * w2->vector[i];

        magnitude1 += w1->vector[i] * w1->vector[i];

        magnitude2 += w2->vector[i] * w2->vector[i];
    }

    magnitude1 = sqrtf(magnitude1);
    magnitude2 = sqrtf(magnitude2);

    if (magnitude1 == 0.0f || magnitude2 == 0.0f)
    {
        return 0.0f;
    }

    similarity = dot_product / (magnitude1 * magnitude2);

    return similarity;
}



int glove_document_vector(GloveModel *model,
                          const char *document,
                          float document_vector[GLOVE_DIM])
{
    char word[256];

    int i;
    int j;
    int length;
    int found_words;

    GloveWord *glove_word;

    if (model == NULL || document == NULL ||
        document_vector == NULL)
    {
        return 0;
    }

    for (i = 0; i < GLOVE_DIM; i++)
    {
        document_vector[i] = 0.0f;
    }

    found_words = 0;
    i = 0;

    while (document[i] != '\0')
    {

        while (document[i] != '\0' &&
               !isalnum((unsigned char)document[i]))
        {
            i++;
        }

        if (document[i] == '\0')
        {
            break;
        }


        length = 0;

        while (document[i] != '\0' &&
               isalnum((unsigned char)document[i]))
        {
            if (length < 255)
            {
                word[length] = (char)tolower(
                    (unsigned char)document[i]
                );

                length++;
            }

            i++;
        }

        word[length] = '\0';

        if (length == 0)
        {
            continue;
        }


        glove_word = glove_lookup(model, word);

        if (glove_word != NULL)
        {
            for (j = 0; j < GLOVE_DIM; j++)
            {
                document_vector[j] +=
                    glove_word->vector[j];
            }

            found_words++;
        }
    }


    if (found_words > 0)
    {
        for (i = 0; i < GLOVE_DIM; i++)
        {
            document_vector[i] =
                document_vector[i] / found_words;
        }
    }

    return found_words;
}



float glove_document_similarity(GloveModel *model,
                                const char *document1,
                                const char *document2)
{
    float vector1[GLOVE_DIM];
    float vector2[GLOVE_DIM];

    float dot_product;
    float magnitude1;
    float magnitude2;

    int count1;
    int count2;
    int i;

    count1 = glove_document_vector(
        model, document1, vector1
    );

    count2 = glove_document_vector(
        model, document2, vector2
    );

    if (count1 == 0 || count2 == 0)
    {
        return -1.0f;
    }

    dot_product = 0.0f;
    magnitude1 = 0.0f;
    magnitude2 = 0.0f;

    for (i = 0; i < GLOVE_DIM; i++)
    {
        dot_product += vector1[i] * vector2[i];

        magnitude1 += vector1[i] * vector1[i];

        magnitude2 += vector2[i] * vector2[i];
    }

    magnitude1 = sqrtf(magnitude1);
    magnitude2 = sqrtf(magnitude2);

    if (magnitude1 == 0.0f || magnitude2 == 0.0f)
    {
        return 0.0f;
    }

    return dot_product / (magnitude1 * magnitude2);
}
