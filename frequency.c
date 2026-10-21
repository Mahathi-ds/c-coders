#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

FreqList *nlp_word_frequency(TokenList *tokens) {
    FreqList *freq_list = malloc(sizeof(FreqList));
    freq_list->capacity = tokens->count;
    freq_list->count = 0;
    freq_list->frequencies = malloc(freq_list->capacity * sizeof(WordFreq));

    for (size_t i = 0; i < tokens->count; i++) {
        int found = 0;
        /* Check if word already exists in our frequency list */
        for (size_t j = 0; j < freq_list->count; j++) {
            if (strcmp(freq_list->frequencies[j].word, tokens->tokens[i]) == 0) {
                freq_list->frequencies[j].count++;
                found = 1;
                break;
            }
        }
        /* If not found, add it as a new word */
        if (!found) {
            freq_list->frequencies[freq_list->count].word = tokens->tokens[i];
            freq_list->frequencies[freq_list->count].count = 1;
            freq_list->count++;
        }
    }
    return freq_list;
}