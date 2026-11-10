#ifndef PROJECT_H
#define PROJECT_H

#include <stddef.h>

/* --- Data Structures --- */

/* Dynamic array structure to hold word tokens */
typedef struct {
    char **tokens;      
    size_t count;       
    size_t capacity;    
} TokenList;

/* Structure to hold a word and its frequency count */
typedef struct {
    char *word;
    int count;
} WordFreq;

/* Dynamic array for frequency analysis */
typedef struct {
    WordFreq *frequencies;
    size_t count;
    size_t capacity;
} FreqList;


/* --- Memory Management --- */
TokenList *nlp_token_list_create(size_t initial_capacity);
void nlp_token_list_free(TokenList *list);


/* --- Text Processing & Algorithms --- */
void nlp_normalize_case(char *text);
TokenList *nlp_tokenize_words(const char *text);
void nlp_stem_word(char *word);
FreqList *nlp_word_frequency(TokenList *tokens);
TokenList *nlp_generate_ngrams(TokenList *tokens, int n);
typedef struct
{
    int digit_removal;
    int punctuation_removal;
    int special_character_removal;
    int extra_space_removal;
} Option;

struct word{
    char data[1000];
    int idx;
};

#define DEFAULT_OPTIONS \
    (Option){1, 1, 1, 1}

#define DIGIT_REMOVAL_FALSE \
    (Option){0, 1, 1, 1}

#define PUNCTUATION_REMOVAL_FALSE \
    (Option){1, 0, 1, 1}

#define SPECIAL_CHARACTER_REMOVAL_FALSE \
    (Option){1, 1, 0, 1}

#define EXTRA_SPACE_REMOVAL_FALSE \
    (Option){1, 1, 1, 0}
void make_lower(char text[]);
void text_normalization(char *text, Option opt);
int sentence_split(char *text,char sentences[][500],int max_sentences);
int calculate_tfidf(const char *files[],int number_of_files,const char *target_word);

typedef enum {
    SENTIMENT_NEGATIVE = -1,
    SENTIMENT_NEUTRAL = 0,
    SENTIMENT_POSITIVE = 1
} Sentiment;

typedef struct {
    int positive_count;
    int negative_count;
    double score;
    Sentiment sentiment;
} SentimentResult;

int sentiment_analyze(
    const TokenList *tokens,
    SentimentResult *result
);

typedef struct {
    char *word;
    size_t frequency;
} Keyword;

/* Keyword list structure */
typedef struct {
    Keyword *keywords;
    size_t count;
    size_t capacity;
} KeywordList;

/* Extract keywords from already-tokenized,
   stop-word-free input tokens */
int nlp_extract_keywords(
    struct word *const tokens[],
    size_t token_count,
    size_t max_keywords,
    KeywordList *result
);

/* Free allocated keyword list memory */
void nlp_keyword_list_free(KeywordList *list);



#endif /* PROJECT_H */
