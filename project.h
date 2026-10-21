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

#endif /* PROJECT_H */