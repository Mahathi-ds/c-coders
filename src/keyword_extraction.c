

// #include <stdlib.h>
// #include <string.h>
// #include <stdint.h>

// #include "project.h"

// /* Find an existing keyword */
// static size_t find_keyword(
//     const KeywordList *list,
//     const char *word
// )
// {
//     for (size_t i = 0; i < list->count; i++) {
//         if (strcmp(list->keywords[i].word, word) == 0)
//             return i;
//     }

//     return list->count;
// }

// /* Increase keyword list capacity */
// static int resize_keyword_list(KeywordList *list)
// {
//     size_t new_capacity;

//     if (list->capacity == 0) {
//         new_capacity = 8;
//     }
//     else {
//         if (list->capacity > SIZE_MAX / 2)
//             return -1;

//         new_capacity = list->capacity * 2;
//     }

//     if (new_capacity > SIZE_MAX / sizeof(Keyword))
//         return -1;

//     Keyword *temp = realloc(
//         list->keywords,
//         new_capacity * sizeof(Keyword)
//     );

//     if (temp == NULL)
//         return -1;

//     list->keywords = temp;
//     list->capacity = new_capacity;

//     return 0;
// }

// /* Add a new keyword */
// static int add_keyword(
//     KeywordList *list,
//     const char *word
// )
// {
//     size_t index = find_keyword(list, word);

//     /* Increment frequency if word already exists */
//     if (index < list->count) {
//         if (list->keywords[index].frequency == SIZE_MAX)
//             return -1;

//         list->keywords[index].frequency++;
//         return 0;
//     }

//     /* Allocate more space if required */
//     if (list->count == list->capacity) {
//         if (resize_keyword_list(list) != 0)
//             return -1;
//     }

//     char *copy = malloc(strlen(word) + 1);

//     if (copy == NULL)
//         return -1;

//     strcpy(copy, word);

//     list->keywords[list->count].word = copy;
//     list->keywords[list->count].frequency = 1;

//     list->count++;

//     return 0;
// }

// /* Sort by descending frequency.
//    Alphabetical order is used for ties. */
// static int compare_keywords(
//     const void *a,
//     const void *b
// )
// {
//     const Keyword *x = (const Keyword *)a;
//     const Keyword *y = (const Keyword *)b;

//     if (x->frequency > y->frequency)
//         return -1;

//     if (x->frequency < y->frequency)
//         return 1;

//     return strcmp(x->word, y->word);
// }

// /* Free keyword list memory */
// void nlp_keyword_list_free(KeywordList *list)
// {
//     if (list == NULL)
//         return;

//     for (size_t i = 0; i < list->count; i++) {
//         free(list->keywords[i].word);
//     }

//     free(list->keywords);

//     list->keywords = NULL;
//     list->count = 0;
//     list->capacity = 0;
// }

// /* Main keyword extraction function */
// int nlp_extract_keywords(
//     struct word *const tokens[],
//     size_t token_count,
//     size_t max_keywords,
//     KeywordList *result
// )
// {
//     if (result == NULL)
//         return -1;

//     if (token_count > 0 && tokens == NULL)
//         return -1;

//     /* Result must be initialized as {0} by the caller */
//     result->keywords = NULL;
//     result->count = 0;
//     result->capacity = 0;

//     /* Process all input tokens */
//     for (size_t i = 0; i < token_count; i++) {

//         if (tokens[i] == NULL)
//             continue;

//         if (tokens[i]->data[0] == '\0')
//             continue;

//         if (add_keyword(result, tokens[i]->data) != 0) {
//             nlp_keyword_list_free(result);
//             return -2;
//         }
//     }

//     /* Sort keywords */
//     if (result->count > 1) {
//         qsort(
//             result->keywords,
//             result->count,
//             sizeof(Keyword),
//             compare_keywords
//         );
//     }

//     /* Limit number of returned keywords if requested */
//     if (max_keywords > 0 && result->count > max_keywords) {

//         for (size_t i = max_keywords; i < result->count; i++) {
//             free(result->keywords[i].word);
//         }

//         result->count = max_keywords;
//     }

//     return 0;
// }

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
