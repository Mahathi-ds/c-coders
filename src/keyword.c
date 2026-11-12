#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "project.h"

/* Sort keywords by descending frequency.
   Alphabetical order is used when frequencies are equal. */
static int compare_keywords(const void *a, const void *b)
{
    const Keyword *x = (const Keyword *)a;
    const Keyword *y = (const Keyword *)b;

    if (x->frequency > y->frequency)
        return -1;

    if (x->frequency < y->frequency)
        return 1;

    return strcmp(x->word, y->word);
}

/* Free keyword list memory */
void nlp_keyword_list_free(KeywordList *list)
{
    if (list == NULL)
        return;

    for (size_t i = 0; i < list->count; i++)
        free(list->keywords[i].word);

    free(list->keywords);

    list->keywords = NULL;
    list->count = 0;
    list->capacity = 0;
}

/* Main keyword extraction function */
int nlp_extract_keywords(
    struct word *const tokens[],
    size_t token_count,
    size_t max_keywords,
    KeywordList *result
)
{
    if (result == NULL)
        return -1;

    if (token_count > 0 && tokens == NULL)
        return -1;

    /* Initialize result */
    result->keywords = NULL;
    result->count = 0;
    result->capacity = 0;

    /* Create temporary TokenList */
    TokenList input;

    input.tokens = NULL;
    input.count = 0;
    input.capacity = token_count;

    if (token_count > 0)
    {
        input.tokens = malloc(token_count * sizeof(char *));

        if (input.tokens == NULL)
            return -2;
    }

    /* Convert struct word tokens into char* tokens */
    for (size_t i = 0; i < token_count; i++)
    {
        if (tokens[i] == NULL)
            continue;

        if (tokens[i]->data[0] == '\0')
            continue;

        input.tokens[input.count++] = tokens[i]->data;
    }

    /* Use existing frequency function */
    FreqList *freq_list = nlp_word_frequency(&input);

    free(input.tokens);

    if (freq_list == NULL)
        return -2;

    /* Determine number of keywords to return */
    size_t keyword_count = freq_list->count;

    if (max_keywords > 0 && keyword_count > max_keywords)
        keyword_count = max_keywords;

    /* Allocate result array */
    if (keyword_count > 0)
    {
        result->keywords = malloc(
            keyword_count * sizeof(Keyword)
        );

        if (result->keywords == NULL)
        {
            free(freq_list->frequencies);
            free(freq_list);
            return -2;
        }
    }

    result->capacity = keyword_count;

    /* Copy frequency results into KeywordList */
    for (size_t i = 0; i < freq_list->count; i++)
    {
        if (result->count == keyword_count)
            break;

        char *copy = malloc(
            strlen(freq_list->frequencies[i].word) + 1
        );

        if (copy == NULL)
        {
            free(freq_list->frequencies);
            free(freq_list);

            nlp_keyword_list_free(result);
            return -2;
        }

        strcpy(copy, freq_list->frequencies[i].word);

        result->keywords[result->count].word = copy;

        result->keywords[result->count].frequency =
            (size_t)freq_list->frequencies[i].count;

        result->count++;
    }

    /* Free frequency list */
    free(freq_list->frequencies);
    free(freq_list);

    /* Sort by descending frequency */
    if (result->count > 1)
    {
        qsort(
            result->keywords,
            result->count,
            sizeof(Keyword),
            compare_keywords
        );
    }

    /* Return success */
    return 0;
}