
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "project.h"


static void initialize_list(FreqList *list)
{
    list->frequencies = NULL;
    list->count = 0;
    list->capacity = 0;
}


static void free_list(FreqList *list)
{
    size_t i;

    for(i = 0; i < list->count; i++)
    {
        free(list->frequencies[i].word);
    }

    free(list->frequencies);

    list->frequencies = NULL;
    list->count = 0;
    list->capacity = 0;
}


static int find_word(FreqList *list, const char *word)
{
    size_t i;

    for(i = 0; i < list->count; i++)
    {
        if(strcmp(list->frequencies[i].word, word) == 0)
        {
            return (int)i;
        }
    }

    return -1;
}


static int add_word(FreqList *list, const char *word)
{
    int position;
    char *new_word;
    WordFreq *temp;

    size_t new_capacity;

    position = find_word(list, word);

    if(position != -1)
    {
        list->frequencies[position].count++;
        return 1;
    }

    if(list->count == list->capacity)
    {
        if(list->capacity == 0)
        {
            new_capacity = 10;
        }
        else
        {
            new_capacity = list->capacity * 2;
        }

        temp = realloc(
            list->frequencies,
            new_capacity * sizeof(WordFreq)
        );

        if(temp == NULL)
        {
            return 0;
        }

        list->frequencies = temp;
        list->capacity = new_capacity;
    }

    new_word = malloc(strlen(word) + 1);

    if(new_word == NULL)
    {
        return 0;
    }

    strcpy(new_word, word);

    list->frequencies[list->count].word = new_word;
    list->frequencies[list->count].count = 1;

    list->count++;

    return 1;
}


static void free_token_stack(struct word **tokens, int count)
{
    int i;

    for(i = 0; i < count; i++)
    {
        free(tokens[i]);
        tokens[i] = NULL;
    }

    rear = 0;
}


static int calculate_word_frequencies(
    const char *filename,
    FreqList *list)
{
    FILE *fp;
    char *text;

    long file_size;
    size_t bytes_read;

    struct word **tokens;

    int token_count;
    int i;

    fp = fopen(filename, "rb");

    if(fp == NULL)
    {
        printf("Cannot open file: %s\n", filename);
        return 0;
    }

    fseek(fp, 0, SEEK_END);

    file_size = ftell(fp);

    if(file_size < 0)
    {
        fclose(fp);
        return 0;
    }

    rewind(fp);

    text = malloc((file_size + 1) * sizeof(char));

    if(text == NULL)
    {
        fclose(fp);
        return 0;
    }

    bytes_read = fread(text, sizeof(char), file_size, fp);

    if(ferror(fp))
    {
        free(text);
        fclose(fp);
        return 0;
    }

    text[bytes_read] = '\0';

    fclose(fp);

    text_normalization(text, DEFAULT_OPTIONS);
    make_lower(text);

    rear = 0;

    tokens = word_tokenize(text);

    token_count = rear;

    free(text);

    if(tokens == NULL)
    {
        return 0;
    }

    for(i = 0; i < token_count; i++)
    {
        if(!add_word(list, tokens[i]->data))
        {
            free_token_stack(tokens, token_count);
            return 0;
        }
    }

    free_token_stack(tokens, token_count);

    return 1;
}


int calculate_cosine_similarity(
    const char *file1,
    const char *file2,
    double *similarity)
{
    FreqList list1;
    FreqList list2;

    size_t i;
    int position;

    double dot_product = 0.0;
    double magnitude1 = 0.0;
    double magnitude2 = 0.0;

    if(file1 == NULL || file2 == NULL || similarity == NULL)
    {
        return 0;
    }

    *similarity = 0.0;

    initialize_list(&list1);
    initialize_list(&list2);

    if(!calculate_word_frequencies(file1, &list1))
    {
        free_list(&list1);
        free_list(&list2);
        return 0;
    }

    if(!calculate_word_frequencies(file2, &list2))
    {
        free_list(&list1);
        free_list(&list2);
        return 0;
    }


    for(i = 0; i < list1.count; i++)
    {
        position = find_word(
            &list2,
            list1.frequencies[i].word
        );

        if(position != -1)
        {
            dot_product +=
                (double)list1.frequencies[i].count *
                list2.frequencies[position].count;
        }
    }


    for(i = 0; i < list1.count; i++)
    {
        magnitude1 +=
            (double)list1.frequencies[i].count *
            list1.frequencies[i].count;
    }

    magnitude1 = sqrt(magnitude1);


    for(i = 0; i < list2.count; i++)
    {
        magnitude2 +=
            (double)list2.frequencies[i].count *
            list2.frequencies[i].count;
    }

    magnitude2 = sqrt(magnitude2);


    if(magnitude1 != 0.0 && magnitude2 != 0.0)
    {
        *similarity =
            dot_product / (magnitude1 * magnitude2);
    }

    free_list(&list1);
    free_list(&list2);

    return 1;
}
