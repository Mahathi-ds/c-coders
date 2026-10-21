#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/project.h"

TokenList *nlp_generate_ngrams(TokenList *tokens, int n) {
    if (n <= 0 || tokens->count < (size_t)n) return NULL;
    
    TokenList *ngram_list = malloc(sizeof(TokenList));
    ngram_list->count = tokens->count - n + 1;
    ngram_list->tokens = malloc(ngram_list->count * sizeof(char *));
    
    for (size_t i = 0; i < ngram_list->count; i++) {
        /* Allocate enough space for n words plus spaces and null terminator */
        ngram_list->tokens[i] = malloc(256 * sizeof(char)); 
        ngram_list->tokens[i][0] = '\0';
        
        for (int j = 0; j < n; j++) {
            strcat(ngram_list->tokens[i], tokens->tokens[i + j]);
            if (j < n - 1) {
                strcat(ngram_list->tokens[i], " ");
            }
        }
    }
    return ngram_list;
}