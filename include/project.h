#ifndef PROJECT_H
#define PROJECT_H

#include <stddef.h>

struct word
{
    char data[1000];
    int idx;
};

typedef struct
{
    int digit_removal;
    int punctuation_removal;
    int special_character_removal;
    int extra_space_removal;
} Option;

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

/* Used by jaccard_similarity.c (inferred from its usage) */
typedef struct
{
    char **words;
    int size;
    int capacity;
} Vocabulary;

typedef struct
{
    Vocabulary vocabulary;
    int *vector1;
    int *vector2;
    int size;
} VectorizerResult;

extern int rear;

int sentence_split(
    char *text,
    char sentences[][SENTENCE_SIZE],
    int max_sentences
);

int make_lower(char text[]);
int make_upper(char text[]);

void text_normalization(char *text, Option opt);

int stop_words(char stopwords[][100]);
int remove_stopwords(struct word **tokens);

/* These two must match tokenize.c exactly */
struct word create(void);
struct word *word_tokenize(char text[]);

int calculate_tfidf(
    const char *filename,
    const char *target_word,
    const char *filenames[],
    int document_count,
    double *score
);

/* ---------- Similarity helpers ---------- */

int sim_find_word(FreqList *list, const char *word);
int sim_build_frequency_list(const char *filename, FreqList *list);
void sim_free_frequency_list(FreqList *list);

/* ---------- Similarity / distance measures ---------- */

int calculate_cosine_similarity(
    const char *file1,
    const char *file2,
    double *similarity
);

int calculate_jaccard_similarity_files(const char *file1,
                                       const char *file2,
                                       double *similarity);
int calculate_dice_similarity(const char *file1,
                              const char *file2,
                              double *similarity);
int calculate_overlap_coefficient(const char *file1,
                                  const char *file2,
                                  double *similarity);
int calculate_euclidean_distance(const char *file1,
                                 const char *file2,
                                 double *distance);
int calculate_manhattan_distance(const char *file1,
                                 const char *file2,
                                 double *distance);

TokenList *nlp_token_list_create(size_t initial_capacity);
void nlp_token_list_free(TokenList *list);

void nlp_normalize_case(char *text);
TokenList *nlp_tokenize_words(const char *text);
void nlp_stem_word(char *word);

FreqList *nlp_word_frequency(TokenList *tokens);
TokenList *nlp_generate_ngrams(TokenList *tokens, int n);

/* ---------- GloVe / semantic similarity ---------- */

#define GLOVE_DIM          100
#define GLOVE_VOCAB_SIZE   400100
#define HASH_TABLE_SIZE    1000003

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
int glove_insert(GloveModel *model, int index);
void glove_free(GloveModel *model);
int glove_load(const char *filename, GloveModel *model);

GloveWord *glove_lookup(GloveModel *model, const char *word);
float glove_word_similarity(GloveModel *model,
                            const char *word1,
                            const char *word2);
int glove_document_vector(GloveModel *model,
                          const char *document,
                          float document_vector[GLOVE_DIM]);
float glove_document_similarity(GloveModel *model,
                                const char *document1,
                                const char *document2);

int calculate_semantic_similarity(const char *file1,
                                  const char *file2,
                                  const char *embedding_file,
                                  double *similarity);

#endif