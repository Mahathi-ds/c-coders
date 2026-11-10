
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// #include "project.h"

// #define MAX_WORD_LENGTH 100
// #define NEGATION_WINDOW 3

// #define POSITIVE_FILE "data/positive_words.txt"
// #define NEGATIVE_FILE "data/negative_words.txt"

// /* Check whether a word exists in a dictionary file */
// static int word_exists(const char *word, const char *filename)
// {
//     FILE *fp = fopen(filename, "r");

//     if (fp == NULL)
//         return -1;

//     char dictionary_word[MAX_WORD_LENGTH];

//     while (fscanf(fp, "%99s", dictionary_word) == 1)
//     {
//         if (strcmp(word, dictionary_word) == 0)
//         {
//             fclose(fp);
//             return 1;
//         }
//     }

//     fclose(fp);
//     return 0;
// }

// /* Check negation words */
// static int is_negation(const char *word)
// {
//     const char *negations[] = {
//         "not", "no", "never", "neither", "nor"
//     };

//     int count = sizeof(negations) / sizeof(negations[0]);

//     for (int i = 0; i < count; i++)
//     {
//         if (strcmp(word, negations[i]) == 0)
//             return 1;
//     }

//     return 0;
// }

// /* Check intensifier words */
// static double get_intensity(const char *word)
// {
//     if (strcmp(word, "very") == 0 ||
//         strcmp(word, "extremely") == 0)
//         return 2.0;

//     if (strcmp(word, "really") == 0 ||
//         strcmp(word, "highly") == 0)
//         return 1.5;

//     return 1.0;
// }

// /* Main sentiment analysis function */
// int sentiment_analyze(
//     const TokenList *tokens,
//     SentimentResult *result)
// {
//     if (tokens == NULL || result == NULL)
//         return -1;

//     if (tokens->count > 0 && tokens->tokens == NULL)
//         return -1;

//     FILE *fp;

//     fp = fopen(POSITIVE_FILE, "r");

//     if (fp == NULL)
//         return -2;

//     fclose(fp);

//     fp = fopen(NEGATIVE_FILE, "r");

//     if (fp == NULL)
//         return -2;

//     fclose(fp);

//     result->positive_count = 0;
//     result->negative_count = 0;
//     result->score = 0.0;
//     result->sentiment = SENTIMENT_NEUTRAL;

//     double total_score = 0.0;
//     double total_magnitude = 0.0;

//     int negation_count = 0;
//     double intensity = 1.0;

//     for (size_t i = 0; i < tokens->count; i++)
//     {
//         if (tokens->tokens[i] == NULL)
//             return -1;

//         char word[MAX_WORD_LENGTH];

//         size_t len = strlen(tokens->tokens[i]);

//         if (len == 0 || len >= MAX_WORD_LENGTH)
//             continue;

//         strcpy(word, tokens->tokens[i]);

//         /* Convert word to lowercase */
//         for (size_t j = 0; word[j] != '\0'; j++)
//         {
//             word[j] = (char)tolower((unsigned char)word[j]);
//         }

//         if (is_negation(word))
//         {
//             negation_count = NEGATION_WINDOW;
//             continue;
//         }

//         double multiplier = get_intensity(word);

//         if (multiplier != 1.0)
//         {
//             intensity = multiplier;
//             continue;
//         }

//         int positive = word_exists(word, POSITIVE_FILE);

//         if (positive == -1)
//             return -2;

//         int negative = word_exists(word, NEGATIVE_FILE);

//         if (negative == -1)
//             return -2;

//         int word_score = 0;

//         if (positive)
//         {
//             word_score = 1;
//             result->positive_count++;
//         }
//         else if (negative)
//         {
//             word_score = -1;
//             result->negative_count++;
//         }

//         if (word_score != 0)
//         {
//             if (negation_count > 0)
//                 word_score *= -1;

//             double weighted_score = word_score * intensity;

//             total_score += weighted_score;
//             total_magnitude += intensity;

//             negation_count = 0;
//             intensity = 1.0;
//         }
//         else
//         {
//             if (negation_count > 0)
//                 negation_count--;

//             intensity = 1.0;
//         }
//     }

//     /* Calculate normalized score */
//     if (total_magnitude > 0)
//     {
//         result->score = total_score / total_magnitude;
//     }

//     /* Classify sentiment */
//     if (result->score > 0.05)
//     {
//         result->sentiment = SENTIMENT_POSITIVE;
//     }
//     else if (result->score < -0.05)
//     {
//         result->sentiment = SENTIMENT_NEGATIVE;
//     }
//     else
//     {
//         result->sentiment = SENTIMENT_NEUTRAL;
//     }

//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#include "project.h"

#define NEGATION_WINDOW 3
#define INTENSITY_WINDOW 2
#define MAX_INTENSITY 3.0

#define POSITIVE_FILE "data/positive_words.txt"
#define NEGATIVE_FILE "data/negative_words.txt"

/* Dynamic dictionary representation */
typedef struct {
    char **words;
    size_t count;
    size_t capacity;
} Dictionary;


/* Case-insensitive comparison */
static int compare_words(const char *a, const char *b)
{
    while (*a && *b) {
        int x = tolower((unsigned char)*a);
        int y = tolower((unsigned char)*b);

        if (x != y)
            return x - y;

        a++;
        b++;
    }

    return tolower((unsigned char)*a) -
           tolower((unsigned char)*b);
}


/* Read a dynamically sized word from a file.
   Returns 1 on success, 0 on EOF, -1 on allocation failure. */
static int read_dynamic_word(FILE *fp, char **output)
{
    size_t capacity = 32;
    size_t length = 0;

    char *word = malloc(capacity);

    if (word == NULL)
        return -1;

    int ch;

    while ((ch = fgetc(fp)) != EOF && isspace((unsigned char)ch))
        ;

    if (ch == EOF) {
        free(word);
        return 0;
    }

    do {
        if (length + 1 >= capacity) {
            if (capacity > (size_t)-1 / 2) {
                free(word);
                return -1;
            }

            size_t new_capacity = capacity * 2;

            char *temp = realloc(word, new_capacity);

            if (temp == NULL) {
                free(word);
                return -1;
            }

            word = temp;
            capacity = new_capacity;
        }

        word[length++] = (char)ch;

        ch = fgetc(fp);

    } while (ch != EOF && !isspace((unsigned char)ch));

    word[length] = '\0';

    *output = word;

    return 1;
}


/* Add word to dynamic dictionary */
static int dictionary_add(Dictionary *dict, char *word)
{
    if (dict->count == dict->capacity) {

        size_t new_capacity =
            dict->capacity == 0 ? 16 : dict->capacity * 2;

        if (new_capacity < dict->capacity ||
            new_capacity > (size_t)-1 / sizeof(char *))
            return -1;

        char **temp = realloc(
            dict->words,
            new_capacity * sizeof(char *)
        );

        if (temp == NULL)
            return -1;

        dict->words = temp;
        dict->capacity = new_capacity;
    }

    dict->words[dict->count++] = word;

    return 0;
}


/* Sort dictionary for faster searching */
static int sort_compare(const void *a, const void *b)
{
    const char *word_a = *(const char * const *)a;
    const char *word_b = *(const char * const *)b;

    return compare_words(word_a, word_b);
}


/* Load dictionary from file only once */
static int dictionary_load(Dictionary *dict, const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        return -1;

    char *word = NULL;
    int status;

    while ((status = read_dynamic_word(fp, &word)) == 1) {

        if (dictionary_add(dict, word) != 0) {
            free(word);
            fclose(fp);
            return -2;
        }
    }

    fclose(fp);

    if (status == -1)
        return -2;

    qsort(
        dict->words,
        dict->count,
        sizeof(char *),
        sort_compare
    );

    return 0;
}


/* Free dictionary memory */
static void dictionary_free(Dictionary *dict)
{
    for (size_t i = 0; i < dict->count; i++)
        free(dict->words[i]);

    free(dict->words);

    dict->words = NULL;
    dict->count = 0;
    dict->capacity = 0;
}


/* Binary search for dictionary word */
static int dictionary_contains(
    const Dictionary *dict,
    const char *word)
{
    size_t left = 0;
    size_t right = dict->count;

    while (left < right) {

        size_t mid = left + (right - left) / 2;

        int comparison = compare_words(
            word,
            dict->words[mid]
        );

        if (comparison == 0)
            return 1;

        if (comparison < 0)
            right = mid;
        else
            left = mid + 1;
    }

    return 0;
}


/* Identify negation words */
static int is_negation(const char *word)
{
    const char *negations[] = {
        "not", "no", "never",
        "neither", "nor", "without"
    };

    size_t count =
        sizeof(negations) / sizeof(negations[0]);

    for (size_t i = 0; i < count; i++) {
        if (compare_words(word, negations[i]) == 0)
            return 1;
    }

    return 0;
}


/* Identify intensifiers */
static double get_intensity(const char *word)
{
    if (compare_words(word, "extremely") == 0 ||
        compare_words(word, "incredibly") == 0)
        return 3.0;

    if (compare_words(word, "very") == 0 ||
        compare_words(word, "absolutely") == 0)
        return 2.0;

    if (compare_words(word, "really") == 0 ||
        compare_words(word, "highly") == 0)
        return 1.5;

    return 1.0;
}


/* Identify diminishers */
static double get_diminisher(const char *word)
{
    if (compare_words(word, "slightly") == 0 ||
        compare_words(word, "barely") == 0)
        return 0.5;

    if (compare_words(word, "somewhat") == 0 ||
        compare_words(word, "fairly") == 0)
        return 0.75;

    return 1.0;
}


/* Remove punctuation surrounding a token */
static const char *clean_token(const char *word, size_t *length)
{
    while (*word && ispunct((unsigned char)*word))
        word++;

    *length = strlen(word);

    while (*length > 0 &&
           ispunct((unsigned char)word[*length - 1]))
        (*length)--;

    return word;
}


/* Check punctuation-only tokens */
static int is_boundary(const char *word)
{
    if (*word == '\0')
        return 0;

    while (*word) {
        if (ispunct((unsigned char)*word))
            word++;
        else
            return 0;
    }

    return 1;
}


/* Main sentiment analysis function */
int sentiment_analyze(
    const TokenList *tokens,
    SentimentResult *result)
{
    if (tokens == NULL || result == NULL)
        return -1;

    if (tokens->count > 0 && tokens->tokens == NULL)
        return -1;

    /* Initialize result */
    result->positive_count = 0;
    result->negative_count = 0;
    result->score = 0.0;
    result->sentiment = SENTIMENT_NEUTRAL;

    if (tokens->count == 0)
        return 0;

    Dictionary positive = {NULL, 0, 0};
    Dictionary negative = {NULL, 0, 0};

    /* Load both dictionaries */
    if (dictionary_load(&positive, POSITIVE_FILE) != 0) {
        dictionary_free(&positive);
        return -2;
    }

    if (dictionary_load(&negative, NEGATIVE_FILE) != 0) {
        dictionary_free(&positive);
        dictionary_free(&negative);
        return -2;
    }

    double total_score = 0.0;
    double total_magnitude = 0.0;

    int negation_window = 0;
    int intensity_window = 0;

    int negated = 0;
    double intensity = 1.0;

    for (size_t i = 0; i < tokens->count; i++) {

        if (tokens->tokens[i] == NULL) {
            dictionary_free(&positive);
            dictionary_free(&negative);
            return -1;
        }

        size_t len;

        const char *word = clean_token(
            tokens->tokens[i], &len
        );

        if (len == 0) {
            negation_window = 0;
            negated = 0;
            intensity = 1.0;
            intensity_window = 0;
            continue;
        }

        /* Reset scope at punctuation boundaries */
        if (is_boundary(tokens->tokens[i])) {
            negation_window = 0;
            negated = 0;
            intensity = 1.0;
            intensity_window = 0;
            continue;
        }

        /* Handle negation */
        if (is_negation(word)) {
            negated = !negated;
            negation_window = NEGATION_WINDOW;
            continue;
        }

        /* Handle intensifiers */
        double boost = get_intensity(word);

        if (boost != 1.0) {
            intensity *= boost;

            if (intensity > MAX_INTENSITY)
                intensity = MAX_INTENSITY;

            intensity_window = INTENSITY_WINDOW;
            continue;
        }

        /* Handle diminishers */
        double reduction = get_diminisher(word);

        if (reduction != 1.0) {
            intensity *= reduction;

            intensity_window = INTENSITY_WINDOW;
            continue;
        }

        int pos = dictionary_contains(&positive, word);
        int neg = dictionary_contains(&negative, word);

        int word_score = 0;

        /* Ignore ambiguous words present in both dictionaries */
        if (pos && !neg)
            word_score = 1;
        else if (neg && !pos)
            word_score = -1;

        if (word_score != 0) {

            /* Apply negation */
            if (negated && negation_window > 0)
                word_score *= -1;

            double weighted_score =
                word_score * intensity;

            total_score += weighted_score;
            total_magnitude += intensity;

            /* Count final polarity */
            if (word_score > 0)
                result->positive_count++;
            else
                result->negative_count++;
        }

        /* Reduce negation scope */
        if (negation_window > 0) {
            negation_window--;

            if (negation_window == 0)
                negated = 0;
        }

        /* Reduce intensity scope */
        if (intensity_window > 0) {
            intensity_window--;

            if (intensity_window == 0)
                intensity = 1.0;
        }
    }

    /* Calculate normalized sentiment score */
    if (total_magnitude > 0.0)
        result->score = total_score / total_magnitude;

    /* Classify sentiment */
    if (result->score > 0.05)
        result->sentiment = SENTIMENT_POSITIVE;

    else if (result->score < -0.05)
        result->sentiment = SENTIMENT_NEGATIVE;

    else
        result->sentiment = SENTIMENT_NEUTRAL;

    /* Release allocated memory */
    dictionary_free(&positive);
    dictionary_free(&negative);

    return 0;
}
