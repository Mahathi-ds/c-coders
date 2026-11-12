
#ifndef PROJECT_H
#define PROJECT_H

#include <stddef.h>

/* ============================================================
   1. BASIC DATA STRUCTURES
   ============================================================ */

struct word
{
    char data[1000];
    int idx;
};

typedef struct
{
    char **tokens;
    size_t count;
    size_t capacity;
} TokenList;

typedef struct
{
    char *word;
    int count;
} WordFreq;

typedef struct
{
    WordFreq *frequencies;
    size_t count;
    size_t capacity;
} FreqList;


/* ============================================================
   2. TEXT NORMALIZATION
   ============================================================ */

typedef struct
{
    int digit_removal;
    int punctuation_removal;
    int special_character_removal;
} Option;

#define DEFAULT_OPTIONS \
    (Option){1, 1, 1}

#define DIGIT_REMOVAL_FALSE \
    (Option){0, 1, 1}

#define PUNCTUATION_REMOVAL_FALSE \
    (Option){1, 0, 1}

#define SPECIAL_CHARACTER_REMOVAL_FALSE \
    (Option){1, 1, 0}

#define KEEP_DIGITS_AND_PUNCTUATION \
    (Option){0, 0, 1}

#define KEEP_DIGITS_AND_SPECIAL \
    (Option){0, 1, 0}

#define KEEP_PUNCTUATION_AND_SPECIAL \
    (Option){1, 0, 0}

#define KEEP_ALL \
    (Option){0, 0, 0}

#define DEFAULT -1
#define SENTENCE_SIZE 500

int make_lower(char text[]);
int make_upper(char text[]);

void text_normalization(char *text, Option opt);

void text_normalization_n(
    char *text,
    size_t size,
    Option opt
);

void expand_contractions(
    const char *src,
    char *dst,
    size_t size
);

int sentence_split(
    char *text,
    char sentences[][SENTENCE_SIZE],
    int max_sentences
);


/* ============================================================
   3. TOKENIZATION AND STOP-WORD REMOVAL
   ============================================================ */

extern int rear;

struct word create(void);

struct word *word_tokenize(char text[]);

int stop_words(const char *filename,char stopwords[][100]);
int remove_stopwords(struct word *tokens,const char *filename,const char **negations,int neg_count);



/* ============================================================
   4. NLP HELPER FUNCTIONS
   ============================================================ */

TokenList *nlp_token_list_create(size_t initial_capacity);

void nlp_token_list_free(TokenList *list);

void nlp_normalize_case(char *text);

TokenList *nlp_tokenize_words(const char *text);

void nlp_stem_word(char *word);

FreqList *nlp_word_frequency(TokenList *tokens);

TokenList *nlp_generate_ngrams(TokenList *tokens, int n);


/* ============================================================
   5. VOCABULARY AND VECTORIZER
   ============================================================ */

typedef struct
{
    char **words;
    int size;
    int capacity;
} Vocab;

typedef Vocab Vocabulary;

typedef struct
{
    Vocabulary vocabulary;
    int *vector1;
    int *vector2;
    int size;
} PairVectorizerResult;

typedef struct
{
    int **vectors;
    int num_docs;
    Vocab vocab;
} VectorizerResult;

VectorizerResult vectoriser(
    char **docs,
    int num_docs
);


/* ============================================================
   6. TF-IDF
   ============================================================ */

int calculate_tfidf(
    const char *filename,
    const char *target_word,
    const char *filenames[],
    int document_count,
    double *score
);


/* ============================================================
   7. SIMILARITY HELPER FUNCTIONS
   ============================================================ */

int sim_find_word(
    FreqList *list,
    const char *word
);

int sim_build_frequency_list(
    const char *filename,
    FreqList *list
);

void sim_free_frequency_list(
    FreqList *list
);


/* ============================================================
   8. SIMILARITY AND DISTANCE MEASURES
   ============================================================ */

int calculate_cosine_similarity(
    const char *file1,
    const char *file2,
    double *similarity
);

int calculate_tfidf_cosine_similarity(
    const char *file1,
    const char *file2,
    double *similarity
);

int calculate_jaccard_similarity_files(
    const char *file1,
    const char *file2,
    double *similarity
);

int calculate_dice_similarity(
    const char *file1,
    const char *file2,
    double *similarity
);

int calculate_overlap_coefficient(
    const char *file1,
    const char *file2,
    double *similarity
);

int calculate_euclidean_distance(
    const char *file1,
    const char *file2,
    double *distance
);

int calculate_manhattan_distance(
    const char *file1,
    const char *file2,
    double *distance
);


/* ============================================================
   9. GLOVE AND SEMANTIC SIMILARITY
   ============================================================ */

#define GLOVE_DIM 100
#define GLOVE_VOCAB_SIZE 400100
#define HASH_TABLE_SIZE 1000003

typedef struct
{
    char *word;
    float vector[GLOVE_DIM];
} GloveWord;

typedef struct
{
    GloveWord *words;
    int *hash_table;
    int word_count;
} GloveModel;

unsigned long glove_hash(const char *word);

int glove_insert(
    GloveModel *model,
    int index
);

void glove_free(GloveModel *model);

int glove_load(
    const char *filename,
    GloveModel *model
);

GloveWord *glove_lookup(
    GloveModel *model,
    const char *word
);

float glove_word_similarity(
    GloveModel *model,
    const char *word1,
    const char *word2
);

int glove_document_vector(
    GloveModel *model,
    const char *document,
    float document_vector[GLOVE_DIM]
);

float glove_document_similarity(
    GloveModel *model,
    const char *document1,
    const char *document2
);

int calculate_semantic_similarity(
    const char *file1,
    const char *file2,
    const char *embedding_file,
    double *similarity
);


/* ============================================================
   10. NAIVE BAYES CLASSIFICATION
   ============================================================ */

typedef struct
{
    char *label;
    int count;
    double prob;
} prior;

typedef struct
{
    char *label;
    WordFreq *word_counts;
    int size;
} ClassWordCounts;

typedef struct
{
    char *class;
    int count;
} classcount;

typedef struct
{
    char *word;
    classcount *classes;
    int size;
} wordclasscounts;

typedef struct
{
    char *label;
    int *doc_indices;
    int count;
} ClassDocuments;

typedef struct
{
    ClassDocuments *classes;
    int num_classes;
} DocumentGroups;

typedef struct
{
    Vocab vocab;
    prior *priors;
    ClassWordCounts *class_word;
    wordclasscounts *word_class;
    int num_classes;
} NaiveBayesModel;

NaiveBayesModel Naivebayes_fit(
    char **docs,
    char **labels,
    int num_docs
);

char **Naivebayes_predict(
    NaiveBayesModel model,
    char **test_docs,
    int num_docs
);

void NaiveBayes_free(
    NaiveBayesModel *model
);


/* ============================================================
   11. SENTIMENT ANALYSIS
   ============================================================ */

typedef enum
{
    SENTIMENT_NEGATIVE = -1,
    SENTIMENT_NEUTRAL = 0,
    SENTIMENT_POSITIVE = 1
} Sentiment;

typedef struct
{
    int positive_count;
    int negative_count;
    double score;
    Sentiment sentiment;
} SentimentResult;

int sentiment_analyze(
    const TokenList *tokens,
    SentimentResult *result
);


/* ============================================================
   12. KEYWORD EXTRACTION
   ============================================================ */

typedef struct
{
    char *word;
    size_t frequency;
} Keyword;

typedef struct
{
    Keyword *keywords;
    size_t count;
    size_t capacity;
} KeywordList;

int nlp_extract_keywords(
    struct word *const tokens[],
    size_t token_count,
    size_t max_keywords,
    KeywordList *result
);

void nlp_keyword_list_free(
    KeywordList *list
);


#endif /* PROJECT_H */
