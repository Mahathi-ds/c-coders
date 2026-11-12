

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#include "../include/project.h"

#define NEGATION_WINDOW 3
#define INTENSITY_WINDOW 2
#define MAX_INTENSITY 3.0
#define MIN_INTENSITY 0.25
#define CONTRAST_WEIGHT 1.0

#define POSITIVE_FILE "data/positive_words.txt"
#define NEGATIVE_FILE "data/negative_words.txt"

typedef struct {
    char **words;
    size_t count;
    size_t capacity;
} Dictionary;

static Dictionary positive = {NULL, 0, 0};
static Dictionary negative = {NULL, 0, 0};

static int dictionaries_initialized = 0;


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


/* Read dynamically sized words */
static int read_dynamic_word(FILE *fp, char **output)
{
    size_t capacity = 32;
    size_t length = 0;

    char *word = malloc(capacity);

    if (word == NULL)
        return -1;

    int ch;

    while ((ch = fgetc(fp)) != EOF &&
           isspace((unsigned char)ch))
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


/* Add word to dictionary */
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


/* Sorting comparator */
static int sort_compare(const void *a, const void *b)
{
    const char *word_a = *(const char * const *)a;
    const char *word_b = *(const char * const *)b;

    return compare_words(word_a, word_b);
}


/* Load dictionary */
static int dictionary_load(
    Dictionary *dict,
    const char *filename)
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

    if (dict->count > 1) {
        qsort(
            dict->words,
            dict->count,
            sizeof(char *),
            sort_compare
        );
    }

    return 0;
}


/* Free dictionary */
static void dictionary_free(Dictionary *dict)
{
    for (size_t i = 0; i < dict->count; i++)
        free(dict->words[i]);

    free(dict->words);

    dict->words = NULL;
    dict->count = 0;
    dict->capacity = 0;
}


/* Copy a user-provided word into dynamically allocated memory. */
static char *copy_word(const char *word)
{
    size_t length = strlen(word);
    char *copy = malloc(length + 1);

    if (copy == NULL)
        return NULL;

    memcpy(copy, word, length + 1);
    return copy;
}


/* Replace a dictionary with words supplied by the caller. */
static int dictionary_set(
    Dictionary *dict,
    const char *words[],
    size_t count)
{
    dictionary_free(dict);

    for (size_t i = 0; i < count; i++) {
        if (words[i] == NULL || words[i][0] == '\0')
            continue;

        char *copy = copy_word(words[i]);

        if (copy == NULL)
            goto error;

        if (dictionary_add(dict, copy) != 0) {
            free(copy);
            goto error;
        }
    }

    if (dict->count > 1) {
        qsort(dict->words, dict->count, sizeof(char *), sort_compare);
    }

    return 0;

error:
    dictionary_free(dict);
    return -1;
}


/* Cleanup at program exit */
static void cleanup_dictionaries(void)
{
    dictionary_free(&positive);
    dictionary_free(&negative);

    dictionaries_initialized = 0;
}


/* Register cleanup only once, whether using files or custom lists. */
static int cleanup_registered = 0;

static int register_dictionary_cleanup(void)
{
    if (cleanup_registered)
        return 0;

    if (atexit(cleanup_dictionaries) != 0)
        return -1;

    cleanup_registered = 1;
    return 0;
}


/* Public API: use positive and negative words supplied by the caller. */
int sentiment_set_word_lists(
    const char *positive_words[],
    size_t positive_count,
    const char *negative_words[],
    size_t negative_count)
{
    if ((positive_count > 0 && positive_words == NULL) ||
        (negative_count > 0 && negative_words == NULL))
        return -1;

    if (register_dictionary_cleanup() != 0)
        return -1;

    dictionary_free(&positive);
    dictionary_free(&negative);
    dictionaries_initialized = 0;

    if (dictionary_set(&positive, positive_words, positive_count) != 0)
        return -1;

    if (dictionary_set(&negative, negative_words, negative_count) != 0) {
        dictionary_free(&positive);
        dictionary_free(&negative);
        return -1;
    }

    dictionaries_initialized = 1;
    return 0;
}


/* Initialize dictionaries once (file-based fallback). */
static int initialize_dictionaries(void)
{
    if (dictionaries_initialized)
        return 0;

    if (dictionary_load(&positive, POSITIVE_FILE) != 0) {
        dictionary_free(&positive);
        return -1;
    }

    if (dictionary_load(&negative, NEGATIVE_FILE) != 0) {
        dictionary_free(&positive);
        dictionary_free(&negative);
        return -1;
    }

    if (register_dictionary_cleanup() != 0) {
        dictionary_free(&positive);
        dictionary_free(&negative);
        return -1;
    }

    dictionaries_initialized = 1;

    return 0;
}


/* Compare token span with dictionary word */
static int compare_span(
    const char *word,
    size_t length,
    const char *dict_word)
{
    size_t i = 0;

    while (i < length && dict_word[i]) {
        int a = tolower((unsigned char)word[i]);
        int b = tolower((unsigned char)dict_word[i]);

        if (a != b)
            return a - b;

        i++;
    }

    if (i < length)
        return 1;

    if (dict_word[i] != '\0')
        return -1;

    return 0;
}


/* Binary search */
static int dictionary_contains(
    const Dictionary *dict,
    const char *word,
    size_t length)
{
    size_t left = 0;
    size_t right = dict->count;

    while (left < right) {
        size_t mid = left + (right - left) / 2;

        int comparison = compare_span(
            word,
            length,
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

/* =========================================
   NEGATION WORDS
   ========================================= */

static int is_negation(
    const char *word,
    size_t length)
{
    static const char *negations[] = {
        "not", "no", "never",
        "neither", "nor", "without",
        "cannot", "cant",
        "dont", "doesnt", "didnt",
        "isnt", "wasnt", "werent",
        "shouldnt", "wouldnt", "couldnt",
        "hasnt", "havent", "hadnt",
        "mustnt", "mightnt", "neednt",
        "nowhere", "nothing", "nobody",
        "none", "hardly", "scarcely"
    };

    size_t count =
        sizeof(negations) / sizeof(negations[0]);

    for (size_t i = 0; i < count; i++) {
        if (compare_span(word, length, negations[i]) == 0)
            return 1;
    }

    return 0;
}


/* =========================================
   INTENSIFIERS
   ========================================= */

static double get_intensity(
    const char *word,
    size_t length)
{
    /* Very strong intensifiers: 3.0 */

    static const char *very_strong[] = {
        "extremely",
        "incredibly",
        "exceptionally",
        "tremendously",
        "immensely",
        "unbelievably",
        "extraordinarily",
        "phenomenally",
        "insanely",
        "remarkably"
    };

    for (size_t i = 0;
         i < sizeof(very_strong) / sizeof(very_strong[0]);
         i++) {

        if (compare_span(word, length, very_strong[i]) == 0)
            return 3.0;
    }


    /* Strong intensifiers: 2.0 */

    static const char *strong[] = {
        "absolutely",
        "completely",
        "totally",
        "utterly",
        "thoroughly",
        "amazingly",
        "highly",
        "super",
        "deeply",
        "fully",
        "entirely",
        "perfectly",
        "especially",
        "terribly",
        "awfully",
        "seriously",
        "hugely",
        "vastly",
        "greatly"
    };

    for (size_t i = 0;
         i < sizeof(strong) / sizeof(strong[0]);
         i++) {

        if (compare_span(word, length, strong[i]) == 0)
            return 2.0;
    }


    /* Moderate intensifiers: 1.5 */

    static const char *moderate[] = {
        "very",
        "really",
        "quite",
        "pretty",
        "particularly",
        "significantly",
        "extensively",
        "strongly",
        "so",
        "considerably",
        "notably",
        "truly"
    };

    for (size_t i = 0;
         i < sizeof(moderate) / sizeof(moderate[0]);
         i++) {

        if (compare_span(word, length, moderate[i]) == 0)
            return 1.5;
    }

    return 1.0;
}


/* =========================================
   DIMINISHERS
   ========================================= */

static double get_diminisher(
    const char *word,
    size_t length)
{
    /* Weak intensity: 0.5 */

    static const char *weak[] = {
        "slightly",
        "mildly",
        "marginally",
        "minimally",
        "barely"
    };

    for (size_t i = 0;
         i < sizeof(weak) / sizeof(weak[0]);
         i++) {

        if (compare_span(word, length, weak[i]) == 0)
            return 0.5;
    }


    /* Moderate reduction: 0.75 */

    static const char *moderate[] = {
        "somewhat",
        "fairly",
        "moderately",
        "relatively",
        "reasonably",
        "partially",
        "rather"
    };

    for (size_t i = 0;
         i < sizeof(moderate) / sizeof(moderate[0]);
         i++) {

        if (compare_span(word, length, moderate[i]) == 0)
            return 0.75;
    }

    return 1.0;
}

/* =========================================
   CONTRAST WORDS
   ========================================= */

static int is_contrast(
    const char *word,
    size_t length)
{
    static const char *contrast_words[] = {
        "but",
        "however",
        "although",
        "though",
        "yet",
        "whereas",
        "nevertheless",
        "nonetheless",
        "still",
        "except"
    };

    size_t count =
        sizeof(contrast_words) / sizeof(contrast_words[0]);

    for (size_t i = 0; i < count; i++) {
        if (compare_span(word, length, contrast_words[i]) == 0)
            return 1;
    }

    return 0;
}


/* =========================================
   CLEAN TOKEN
   ========================================= */

static const char *clean_token(
    const char *word,
    size_t *length)
{
    while (*word && ispunct((unsigned char)*word))
        word++;

    *length = strlen(word);

    while (*length > 0 &&
           ispunct((unsigned char)word[*length - 1])) {
        (*length)--;
    }

    return word;
}


/* =========================================
   CHECK PUNCTUATION-ONLY TOKEN
   ========================================= */

static int is_boundary(const char *word)
{
    if (*word == '\0')
        return 0;

    while (*word) {
        if (!ispunct((unsigned char)*word))
            return 0;

        word++;
    }

    return 1;
}


/* =========================================
   CHECK SENTENCE ENDING
   ========================================= */

static int ends_sentence(const char *word)
{
    size_t len = strlen(word);

    while (len > 0 &&
           ispunct((unsigned char)word[len - 1])) {

        char ch = word[len - 1];

        if (ch == '.' || ch == '!' || ch == '?')
            return 1;

        len--;
    }

    return 0;
}


/* =========================================
   RESET NEGATION AND INTENSITY SCOPES
   ========================================= */

static void reset_scopes(
    int *negation_window,
    int *negated,
    double *intensity,
    int *intensity_window,
    int *contrast_active)
{
    *negation_window = 0;
    *negated = 0;

    *intensity = 1.0;
    *intensity_window = 0;

    *contrast_active = 0;
}

/* =========================================
   MAIN SENTIMENT ANALYSIS FUNCTION
   ========================================= */

int sentiment_analyze(
    const TokenList *tokens,
    SentimentResult *result)
{
    if (tokens == NULL || result == NULL)
        return -1;

    if (tokens->count > 0 && tokens->tokens == NULL)
        return -1;

    result->positive_count = 0;
    result->negative_count = 0;
    result->score = 0.0;
    result->sentiment = SENTIMENT_NEUTRAL;

    if (tokens->count == 0)
        return 0;

    /* Initialize dictionaries */

    if (initialize_dictionaries() != 0)
        return -2;


    double total_score = 0.0;
    double total_magnitude = 0.0;

    int negation_window = 0;
    int intensity_window = 0;

    int negated = 0;
    int contrast_active = 0;

    double intensity = 1.0;


    /* Process each token */

    for (size_t i = 0; i < tokens->count; i++) {

        if (tokens->tokens[i] == NULL)
            return -1;

        const char *original = tokens->tokens[i];


        /* Handle punctuation-only tokens */

        if (is_boundary(original)) {

            if (ends_sentence(original)) {

                reset_scopes(
                    &negation_window,
                    &negated,
                    &intensity,
                    &intensity_window,
                    &contrast_active
                );
            }

            continue;
        }


        size_t len;

        const char *word = clean_token(original, &len);

        if (len == 0)
            continue;


        /* Handle contrast words */

        if (is_contrast(word, len)) {

            contrast_active = 1;

            /* Reset modifier scopes after contrast */

            negation_window = 0;
            negated = 0;

            intensity = 1.0;
            intensity_window = 0;

            goto sentence_check;
        }


        /* Handle negation */

        if (is_negation(word, len)) {

            negated = 1;
            negation_window = NEGATION_WINDOW;

            goto sentence_check;
        }


        /* Handle intensifiers */

        double boost = get_intensity(word, len);

        if (boost != 1.0) {

            intensity *= boost;

            if (intensity > MAX_INTENSITY)
                intensity = MAX_INTENSITY;

            intensity_window = INTENSITY_WINDOW;

            goto sentence_check;
        }


        /* Handle diminishers */

        double reduction = get_diminisher(word, len);

        if (reduction != 1.0) {

            intensity *= reduction;

            if (intensity < MIN_INTENSITY)
                intensity = MIN_INTENSITY;

            intensity_window = INTENSITY_WINDOW;

            goto sentence_check;
        }


        /* Dictionary lookup */

        int pos = dictionary_contains(
            &positive, word, len
        );

        int neg = dictionary_contains(
            &negative, word, len
        );

        int word_score = 0;


        /* Ignore ambiguous words */

        if (pos && !neg)
            word_score = 1;

        else if (neg && !pos)
            word_score = -1;


        if (word_score != 0) {

            /* Apply negation */

            if (negated && negation_window > 0)
                word_score *= -1;


            /* Apply contrast weight */

            double contrast_weight =
                contrast_active ? CONTRAST_WEIGHT : 1.0;


            /* Calculate weighted score */

            double weighted_score =
                word_score * intensity * contrast_weight;

            total_score += weighted_score;

            total_magnitude +=
                intensity * contrast_weight;


            /* Count final polarity */

            if (word_score > 0)
                result->positive_count++;

            else
                result->negative_count++;
        }


        // /* Reduce negation scope */

        // if (negation_window > 0) {

        //     negation_window--;

        //     if (negation_window == 0)
        //         negated = 0;
        // }


        // /* Reduce intensity scope */

        // if (intensity_window > 0) {

        //     intensity_window--;

        //     if (intensity_window == 0)
        //         intensity = 1.0;
        // }
        /* Reduce negation scope after sentiment words */

        if (word_score != 0 && negation_window > 0) {

            negation_window--;

            if (negation_window == 0)
                negated = 0;
        }


        /* Reduce intensity scope after ordinary tokens */

        if (intensity_window > 0) {

            intensity_window--;

            if (intensity_window == 0)
                intensity = 1.0;
        }

        /* Sentence boundary */

        sentence_check:

        if (ends_sentence(original)) {

            reset_scopes(
                &negation_window,
                &negated,
                &intensity,
                &intensity_window,
                &contrast_active
            );
        }
    }


    /* Calculate normalized sentiment score */

    if (total_magnitude > 0.0) {

        result->score =
            total_score / total_magnitude;
    }


    /* Final sentiment classification */

    if (result->score > 0.05) {

        result->sentiment = SENTIMENT_POSITIVE;

    }
    else if (result->score < -0.05) {

        result->sentiment = SENTIMENT_NEGATIVE;

    }
    else {

        result->sentiment = SENTIMENT_NEUTRAL;
    }


    return 0;
}

