
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project.h"


/* Find a word in the vocabulary */

int find_word_jacc(char *arr[], int size, char *word)
{
    int i;

    for (i = 0; i < size; i++)
    {
        if (strcmp(arr[i], word) == 0)
        {
            return i;
        }
    }

    return -1;
}


/* Copy tokens from the tokenizer's global stack */

static char **copy_tokens(char *text, int *count)
{
    struct word *tokens;
    char **words;

    int i;

    rear = 0;

    tokens = word_tokenize(text);

    if (tokens == NULL)
    {
        *count = 0;
        return NULL;
    }

    *count = rear;

    if (*count == 0)
    {
        return NULL;
    }

    words = malloc(*count * sizeof(char *));

    if (words == NULL)
    {
        *count = 0;
        return NULL;
    }

    for (i = 0; i < *count; i++)
    {
        words[i] = malloc(strlen(tokens[i].data) + 1);

        if (words[i] == NULL)
        {
            int j;

            for (j = 0; j < i; j++)
            {
                free(words[j]);
            }

            free(words);
            *count = 0;
            return NULL;
        }

        strcpy(words[i], tokens[i].data);
    }

    return words;
}


/* Free copied tokens */

static void free_copied_tokens(char **words, int count)
{
    int i;

    if (words == NULL)
    {
        return;
    }

    for (i = 0; i < count; i++)
    {
        free(words[i]);
    }

    free(words);
}


/* Free a VectorizerResult */

static void free_vectorizer(VectorizerResult *result)
{
    int i;

    if (result == NULL)
    {
        return;
    }

    for (i = 0; i < result->vocabulary.size; i++)
    {
        free(result->vocabulary.words[i]);
    }

    free(result->vocabulary.words);
    free(result->vector1);
    free(result->vector2);
    free(result);
}


/* Add a unique word to the vocabulary */

static int add_vocabulary_word(
    VectorizerResult *result,
    const char *word)
{
    char **temp;
    char *new_word;

    int index;

    index = find_word_jacc(
        result->vocabulary.words,
        result->vocabulary.size,
        (char *)word
    );

    if (index != -1)
    {
        return 1;
    }

    new_word = malloc(strlen(word) + 1);

    if (new_word == NULL)
    {
        return 0;
    }

    strcpy(new_word, word);

    temp = realloc(
        result->vocabulary.words,
        (result->vocabulary.size + 1) * sizeof(char *)
    );

    if (temp == NULL)
    {
        free(new_word);
        return 0;
    }

    result->vocabulary.words = temp;

    result->vocabulary.words[result->vocabulary.size] = new_word;

    result->vocabulary.size++;

    return 1;
}


/* Mark words present in a document */

static void mark_document_words(
    char **words,
    int count,
    VectorizerResult *result,
    int *vector)
{
    int i;
    int index;

    for (i = 0; i < count; i++)
    {
        index = find_word_jacc(
            result->vocabulary.words,
            result->vocabulary.size,
            words[i]
        );

        if (index != -1)
        {
            vector[index] = 1;
        }
    }
}


/* Create a common vocabulary and binary vectors */

VectorizerResult *binary_vectorizer(char *doc1, char *doc2)
{
    char **tokens1;
    char **tokens2;

    int count1;
    int count2;

    int i;

    VectorizerResult *result;

    if (doc1 == NULL || doc2 == NULL)
    {
        return NULL;
    }

    /*
       Copy the first document's tokens.
       The tokenizer's stack will be overwritten
       when the second document is tokenized.
    */

    tokens1 = copy_tokens(doc1, &count1);

    if (count1 > 0 && tokens1 == NULL)
    {
        return NULL;
    }

    /*
       Copy the second document's tokens.
    */

    tokens2 = copy_tokens(doc2, &count2);

    if (count2 > 0 && tokens2 == NULL)
    {
        free_copied_tokens(tokens1, count1);
        return NULL;
    }

    result = malloc(sizeof(VectorizerResult));

    if (result == NULL)
    {
        free_copied_tokens(tokens1, count1);
        free_copied_tokens(tokens2, count2);
        return NULL;
    }

    result->vocabulary.words = NULL;
    result->vocabulary.size = 0;

    result->vector1 = NULL;
    result->vector2 = NULL;

    result->size = 0;

    /*
       Add unique words from document 1.
    */

    for (i = 0; i < count1; i++)
    {
        if (!add_vocabulary_word(result, tokens1[i]))
        {
            free_copied_tokens(tokens1, count1);
            free_copied_tokens(tokens2, count2);
            free_vectorizer(result);
            return NULL;
        }
    }

    /*
       Add unique words from document 2.
    */

    for (i = 0; i < count2; i++)
    {
        if (!add_vocabulary_word(result, tokens2[i]))
        {
            free_copied_tokens(tokens1, count1);
            free_copied_tokens(tokens2, count2);
            free_vectorizer(result);
            return NULL;
        }
    }

    result->size = result->vocabulary.size;

    /*
       Allocate binary vectors.
    */

    if (result->size > 0)
    {
        result->vector1 = calloc(result->size, sizeof(int));
        result->vector2 = calloc(result->size, sizeof(int));

        if (result->vector1 == NULL ||
            result->vector2 == NULL)
        {
            free_copied_tokens(tokens1, count1);
            free_copied_tokens(tokens2, count2);
            free_vectorizer(result);
            return NULL;
        }

        /*
           Mark the words present in each document.
        */

        mark_document_words(
            tokens1,
            count1,
            result,
            result->vector1
        );

        mark_document_words(
            tokens2,
            count2,
            result,
            result->vector2
        );
    }

    free_copied_tokens(tokens1, count1);
    free_copied_tokens(tokens2, count2);

    return result;
}


/* Calculate Jaccard similarity between two text strings */

double jaccard(char *doc1, char *doc2)
{
    VectorizerResult *result;

    int intersection = 0;
    int union_count = 0;

    int i;

    double similarity;

    result = binary_vectorizer(doc1, doc2);

    if (result == NULL)
    {
        return -1.0;
    }

    for (i = 0; i < result->size; i++)
    {
        if (result->vector1[i] == 1 &&
            result->vector2[i] == 1)
        {
            intersection++;
        }

        if (result->vector1[i] == 1 ||
            result->vector2[i] == 1)
        {
            union_count++;
        }
    }

    if (union_count == 0)
    {
        similarity = 1.0;
    }
    else
    {
        similarity = (double)intersection / union_count;
    }

    free_vectorizer(result);

    return similarity;
}


/* Read a complete file into dynamically allocated memory */

static char *read_jaccard_file(const char *filename)
{
    FILE *fp;

    char *text;
    long file_size;

    size_t bytes_read;

    if (filename == NULL)
    {
        return NULL;
    }

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


/* Calculate Jaccard similarity using two filenames */

int calculate_jaccard_similarity_files(
    const char *file1,
    const char *file2,
    double *similarity)
{
    char *text1;
    char *text2;

    if (file1 == NULL ||
        file2 == NULL ||
        similarity == NULL)
    {
        return 0;
    }

    *similarity = 0.0;

    text1 = read_jaccard_file(file1);

    if (text1 == NULL)
    {
        return 0;
    }

    text2 = read_jaccard_file(file2);

    if (text2 == NULL)
    {
        free(text1);
        return 0;
    }

    text_normalization(text1, DEFAULT_OPTIONS);
    text_normalization(text2, DEFAULT_OPTIONS);

    make_lower(text1);
    make_lower(text2);

    *similarity = jaccard(text1, text2);

    free(text1);
    free(text2);

    if (*similarity < 0.0)
    {
        return 0;
    }

    return 1;
}
