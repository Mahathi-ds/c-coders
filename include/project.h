
#ifndef PROJECT_H
#define PROJECT_H

struct word{
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

int sentence_split(char *text, char sentences[][SENTENCE_SIZE],int max_sentences);
int make_lower(char text[]);
int make_upper(char text[]);

void text_normalization(char *text, Option opt);
int sentence_split(char *text,char sentences[][500],int max_sentences);
int calculate_tfidf(const char *files[],int number_of_files,const char *target_word);

int stop_words(char stopwords[][100]);
int remove_stopwords(struct word **tokens);

extern int rear;
struct word *create(void);
struct word **word_tokenize(char text[]);


#include <stddef.h>

typedef struct {
    char **tokens;      
    size_t count;       
    size_t capacity;    
} TokenList;

typedef struct {
    char *word;
    int count;
} WordFreq;

typedef struct {
    WordFreq *frequencies;
    size_t count;
    size_t capacity;
} FreqList;


TokenList *nlp_token_list_create(size_t initial_capacity);
void nlp_token_list_free(TokenList *list);


void nlp_normalize_case(char *text);
TokenList *nlp_tokenize_words(const char *text);
void nlp_stem_word(char *word);
FreqList *nlp_word_frequency(TokenList *tokens);
TokenList *nlp_generate_ngrams(TokenList *tokens, int n);

#endif
