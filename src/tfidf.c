
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "project.h"


/* Read a complete file */

static char *read_file(const char *filename)
{
    FILE *fp;
    char *text;

    long file_size;
    size_t bytes_read;

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("Cannot open file: %s\n", filename);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);

    file_size = ftell(fp);

    if (file_size < 0)
    {
        fclose(fp);
        return NULL;
    }

    rewind(fp);

    text = malloc((size_t)file_size + 1);

    if (text == NULL)
    {
        fclose(fp);
        return NULL;
    }

    bytes_read = fread(text, 1, (size_t)file_size, fp);

    if (ferror(fp))
    {
        free(text);
        fclose(fp);
        return NULL;
    }

    text[bytes_read] = '\0';

    fclose(fp);

    return text;
}


/* Calculate term frequency */

static double calculate_tf(
    const char *filename,
    const char *target_word)
{
    char *text;

    struct word *tokens;

    int word_count = 0;
    int token_count;
    int i;

    double tf = 0.0;

    text = read_file(filename);

    if (text == NULL)
    {
        return -1.0;
    }

    text_normalization(text, DEFAULT_OPTIONS);
    make_lower(text);

    rear = 0;

    tokens = word_tokenize(text);

    token_count = rear;

    free(text);

    if (tokens == NULL)
    {
        return -1.0;
    }

    for (i = 0; i < token_count; i++)
    {
        if (strcmp(tokens[i].data, target_word) == 0)
        {
            word_count++;
        }
    }

    if (token_count > 0)
    {
        tf = (double)word_count / token_count;
    }

    return tf;
}


/* Calculate document frequency */

static int calculate_df(
    const char *target_word,
    const char *filenames[],
    int document_count)
{
    int df = 0;
    int found;

    int i;
    int j;
    int token_count;

    for (i = 0; i < document_count; i++)
    {
        char *text;
        struct word *tokens;

        if (filenames[i] == NULL)
        {
            return -1;
        }

        text = read_file(filenames[i]);

        if (text == NULL)
        {
            return -1;
        }

        text_normalization(text, DEFAULT_OPTIONS);
        make_lower(text);

        rear = 0;

        tokens = word_tokenize(text);

        token_count = rear;

        free(text);

        if (tokens == NULL)
        {
            return -1;
        }

        found = 0;

        for (j = 0; j < token_count; j++)
        {
            if (strcmp(tokens[j].data, target_word) == 0)
            {
                found = 1;
                break;
            }
        }

        if (found == 1)
        {
            df++;
        }
    }

    return df;
}


/* Calculate TF-IDF */

int calculate_tfidf(
    const char *filename,
    const char *target_word,
    const char *filenames[],
    int document_count,
    double *score)
{
    double tf;
    double idf;

    int df;

    char *word;
    size_t word_length;

    if (filename == NULL ||
        target_word == NULL ||
        filenames == NULL ||
        score == NULL ||
        document_count <= 0 ||
        target_word[0] == '\0')
    {
        return 0;
    }

    *score = 0.0;

    word_length = strlen(target_word);

    word = malloc(word_length + 1);

    if (word == NULL)
    {
        return 0;
    }

    strcpy(word, target_word);

    text_normalization(word, DEFAULT_OPTIONS);
    make_lower(word);

    tf = calculate_tf(filename, word);

    if (tf < 0.0)
    {
        free(word);
        return 0;
    }

    df = calculate_df(
        word,
        filenames,
        document_count
    );

    if (df < 0)
    {
        free(word);
        return 0;
    }

    if (df == 0)
    {
        idf = 0.0;
    }
    else
    {
        idf = log((double)document_count / df);
    }

    *score = tf * idf;

    free(word);

    return 1;
}
