
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "project.h"


/* Initialize a frequency list */

static void initialize_list(FreqList *list)
{
    list->frequencies = NULL;
    list->count = 0;
    list->capacity = 0;
}


/* Free a frequency list */

static void free_list(FreqList *list)
{
    int i;

    for (i = 0; i < list->count; i++)
    {
        free(list->frequencies[i].word);
    }

    free(list->frequencies);

    list->frequencies = NULL;
    list->count = 0;
    list->capacity = 0;
}


/* Find a word in a frequency list */

static int find_word(FreqList *list, const char *word)
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


/* Add a word to the frequency list */

static int add_word(FreqList *list, const char *word)
{
    int position;
    char *new_word;
    WordFreq *temp;

    size_t new_capacity;

    position = find_word(list, word);

    if (position != -1)
    {
        list->frequencies[position].count++;
        return 1;
    }

    if (list->count == list->capacity)
    {
        if (list->capacity == 0)
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


/* Calculate word frequencies from a document */

static int calculate_word_frequencies(
    const char *filename,
    FreqList *list)
{
    FILE *fp;

    char *text;
    long file_size;
    size_t bytes_read;

    struct word *tokens;

    int token_count;
    int i;

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("Cannot open file: %s\n", filename);
        return 0;
    }

    fseek(fp, 0, SEEK_END);

    file_size = ftell(fp);

    if (file_size < 0)
    {
        fclose(fp);
        return 0;
    }

    rewind(fp);

    text = malloc((size_t)file_size + 1);

    if (text == NULL)
    {
        fclose(fp);
        return 0;
    }

    bytes_read = fread(text, 1, (size_t)file_size, fp);

    if (ferror(fp))
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

    if (tokens == NULL)
    {
        return 0;
    }

    /*
       The tokenizer returns a static array.
       Therefore, do not free tokens or tokens[i].
    */

    for (i = 0; i < token_count; i++)
    {
        if (!add_word(list, tokens[i].data))
        {
            return 0;
        }
    }

    return 1;
}


/* Calculate cosine similarity between two documents */

int calculate_cosine_similarity(
    const char *file1,
    const char *file2,
    double *similarity)
{
    FreqList list1;
    FreqList list2;

    int i;
    int position;

    double dot_product = 0.0;
    double magnitude1 = 0.0;
    double magnitude2 = 0.0;

    if (file1 == NULL ||
        file2 == NULL ||
        similarity == NULL)
    {
        return 0;
    }

    *similarity = 0.0;

    initialize_list(&list1);
    initialize_list(&list2);

    if (!calculate_word_frequencies(file1, &list1))
    {
        free_list(&list1);
        free_list(&list2);
        return 0;
    }

    if (!calculate_word_frequencies(file2, &list2))
    {
        free_list(&list1);
        free_list(&list2);
        return 0;
    }

    /* Calculate dot product */

    for (i = 0; i < list1.count; i++)
    {
        position = find_word(
            &list2,
            list1.frequencies[i].word
        );

        if (position != -1)
        {
            dot_product +=
                (double)list1.frequencies[i].count *
                list2.frequencies[position].count;
        }
    }

    /* Calculate magnitude of document 1 */

    for (i = 0; i < list1.count; i++)
    {
        magnitude1 +=
            (double)list1.frequencies[i].count *
            list1.frequencies[i].count;
    }

    magnitude1 = sqrt(magnitude1);

    /* Calculate magnitude of document 2 */

    for (i = 0; i < list2.count; i++)
    {
        magnitude2 +=
            (double)list2.frequencies[i].count *
            list2.frequencies[i].count;
    }

    magnitude2 = sqrt(magnitude2);

    /* Calculate cosine similarity */

    if (magnitude1 != 0.0 && magnitude2 != 0.0)
    {
        *similarity =
            dot_product / (magnitude1 * magnitude2);
    }

    free_list(&list1);
    free_list(&list2);

    return 1;
}
