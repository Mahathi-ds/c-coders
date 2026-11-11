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
#define KEEP_DIGITS_AND_PUNCTUATION \
    (Option){0, 0, 1, 1}

#define KEEP_DIGITS_AND_SPECIAL \
    (Option){0, 1, 0, 1}

#define KEEP_PUNCTUATION_AND_SPECIAL \
    (Option){1, 0, 0, 1}

#define KEEP_ALL \
    (Option){0, 0, 0, 1}

#define DEFAULT -1
#define SENTENCE_SIZE 500
int make_lower(char text[]);
int make_upper(char text[]);
/* Expands contractions (isn't -> is not), then applies the Option rules.
 * Expansion makes text longer: the buffer must have room for about 2x the
 * current length. Use text_normalization_n() if you know the real size. */
void text_normalization(char *text, Option opt);
void text_normalization_n(char *text, size_t size, Option opt);

/* Writes the contraction-expanded copy of src into dst (at most size bytes) */
void expand_contractions(const char *src, char *dst, size_t size);
int sentence_split(char *text,char sentences[][500],int max_sentences);

int stop_words(char stopwords[][100]);
int remove_stopwords(struct word *tokens,int keep_negations);

extern int rear;
struct word create(void);
struct word *word_tokenize(char text[]);

typedef struct {
    char **words;
    int size;
    int capacity;
} Vocab;

typedef struct {
    int **vectors;
    int num_docs;
    Vocab vocab;

} VectorizerResult;

typedef struct{
    char *label;
    int count;
    double prob;
}prior;



typedef struct {
    char *label;
    WordFreq *word_counts;
    int size;
} ClassWordCounts;

typedef struct{
    char *class;
    int count;
}classcount;

typedef struct {
    char *word;
    classcount *classes;
    int size;
}wordclasscounts;


typedef struct {
    char *label;
    int *doc_indices;
    int count;
} ClassDocuments;

typedef struct {
    ClassDocuments *classes;
    int num_classes;
} DocumentGroups;


typedef struct {
    Vocab vocab;
    prior *priors;
    ClassWordCounts *class_word;
    wordclasscounts *word_class;
    int num_classes;
} NaiveBayesModel;

VectorizerResult vectoriser(char **docs,int num_docs);
NaiveBayesModel Naivebayes_fit(char **docs, char **labels, int num_docs);
char **Naivebayes_predict( NaiveBayesModel model, char **test_docs, int num_docs);

void NaiveBayes_free(NaiveBayesModel *model);


// sentiment

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
