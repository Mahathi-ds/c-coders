
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project.h"



unsigned long glove_hash(const char *word)
{
    unsigned long hash = 2166136261u;

    while (*word != '\0')
    {
        hash = hash ^ (unsigned char)(*word);
        hash = hash * 16777619u;

        word++;
    }

    return hash % HASH_TABLE_SIZE;
}



int glove_insert(GloveModel *model, int index)
{
    unsigned long position;
    int i;

    if (model == NULL || model->words == NULL ||
        model->hash_table == NULL)
    {
        return 0;
    }

    if (index < 0 || index >= model->word_count)
    {
        return 0;
    }

    position = glove_hash(model->words[index].word);

    for (i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (model->hash_table[position] == 0)
        {
            model->hash_table[position] = index + 1;

            return 1;
        }

        else
        {
            int existing_index;

            existing_index = model->hash_table[position] - 1;

            if (strcmp(model->words[existing_index].word,
                       model->words[index].word) == 0)
            {
                return 1;
            }
        }

        position = (position + 1) % HASH_TABLE_SIZE;
    }

    return 0;
}



void glove_free(GloveModel *model)
{
    int i;

    if (model == NULL)
    {
        return;
    }

    if (model->words != NULL)
    {
        for (i = 0; i < model->word_count; i++)
        {
            free(model->words[i].word);
        }
    }

    free(model->words);
    free(model->hash_table);

    model->words = NULL;
    model->hash_table = NULL;
    model->word_count = 0;
}





int glove_load(const char *filename, GloveModel *model)
{
    FILE *fp;

    char line[4096];
    char word[256];
    char *token;
    char *endptr;

    float vector[GLOVE_DIM];

    int dimension;
    int valid_dimensions;
    int i;
    int line_number;

    if (filename == NULL || model == NULL)
    {
        printf("Error: Invalid filename or model.\n");
        return 0;
    }

    model->words = NULL;
    model->hash_table = NULL;
    model->word_count = 0;

    printf("Opening file: %s\n", filename);

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: Could not open GloVe file.\n");
        return 0;
    }

    printf("GloVe file opened successfully.\n");

    model->words = calloc(
        GLOVE_VOCAB_SIZE, sizeof(GloveWord)
    );

    if (model->words == NULL)
    {
        printf("Error: Word memory allocation failed.\n");
        fclose(fp);
        return 0;
    }

    model->hash_table = calloc(
        HASH_TABLE_SIZE, sizeof(int)
    );

    if (model->hash_table == NULL)
    {
        printf("Error: Hash table allocation failed.\n");
        fclose(fp);
        glove_free(model);
        return 0;
    }

    line_number = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line_number++;

        token = strtok(line, " \t\r\n");

        if (token == NULL)
        {
            continue;
        }

        if (strlen(token) >= sizeof(word))
        {
            printf("Error: Word too long at line %d.\n",
                   line_number);

            fclose(fp);
            glove_free(model);
            return 0;
        }

        strcpy(word, token);

        if (model->word_count >= GLOVE_VOCAB_SIZE)
        {
            printf("Error: Vocabulary limit exceeded.\n");
            printf("Maximum capacity: %d\n", GLOVE_VOCAB_SIZE);
            printf("Current word count: %d\n", model->word_count);

            fclose(fp);
            glove_free(model);
            return 0;
        }

        valid_dimensions = 1;

        for (dimension = 0;
             dimension < GLOVE_DIM;
             dimension++)
        {
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
            {
                valid_dimensions = 0;
                break;
            }

            vector[dimension] = strtof(token, &endptr);

            if (endptr == token || *endptr != '\0')
            {
                valid_dimensions = 0;
                break;
            }
        }

        if (valid_dimensions == 0)
        {
            printf("Error: Invalid vector at line %d.\n",
                   line_number);

            fclose(fp);
            glove_free(model);
            return 0;
        }

        i = model->word_count;

        model->words[i].word = malloc(strlen(word) + 1);

        if (model->words[i].word == NULL)
        {
            printf("Error: Memory allocation failed at line %d.\n",
                   line_number);

            fclose(fp);
            glove_free(model);
            return 0;
        }

        strcpy(model->words[i].word, word);

        for (dimension = 0;
             dimension < GLOVE_DIM;
             dimension++)
        {
            model->words[i].vector[dimension] =
                vector[dimension];
        }

        model->word_count++;

        if (glove_insert(model, i) == 0)
        {
            printf("Error: Hash insertion failed at line %d.\n",
                   line_number);

            fclose(fp);
            glove_free(model);
            return 0;
        }

        if (model->word_count % 50000 == 0)
        {
            printf("Loaded %d words...\n", model->word_count);
        }
    }

    fclose(fp);

    printf("\nGloVe vocabulary loaded successfully!\n");
    printf("Total words: %d\n", model->word_count);

    return 1;
}

/* Helper function to read a document */

static char *semantic_read_file(const char *filename)
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
    rewind(fp);

    if (file_size < 0)
    {
        fclose(fp);
        return NULL;
    }

    text = malloc((size_t)file_size + 1);

    if (text == NULL)
    {
        fclose(fp);
        return NULL;
    }

    bytes_read = fread(text, 1, (size_t)file_size, fp);

    if (bytes_read != (size_t)file_size)
    {
        free(text);
        fclose(fp);
        return NULL;
    }

    text[bytes_read] = '\0';

    fclose(fp);

    return text;
}


/* Semantic Similarity Between Two Documents */

int calculate_semantic_similarity(
    const char *file1,
    const char *file2,
    const char *embedding_file,
    double *similarity
)
{
    char *document1;
    char *document2;

    GloveModel model;

    float score;

    if (file1 == NULL || file2 == NULL ||
        embedding_file == NULL || similarity == NULL)
    {
        return 0;
    }

    document1 = semantic_read_file(file1);

    if (document1 == NULL)
    {
        return 0;
    }

    document2 = semantic_read_file(file2);

    if (document2 == NULL)
    {
        free(document1);
        return 0;
    }

    model.words = NULL;
    model.hash_table = NULL;
    model.word_count = 0;

    if (glove_load(embedding_file, &model) != 1)
    {
        free(document1);
        free(document2);
        return 0;
    }

    score = glove_document_similarity(
        &model,
        document1,
        document2
    );

    if (score < 0.0f)
    {
        glove_free(&model);
        free(document1);
        free(document2);
        return 0;
    }

    *similarity = score;

    glove_free(&model);

    free(document1);
    free(document2);

    return 1;
}
