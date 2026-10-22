
#include <stdlib.h>
#include <string.h>
#include "../include/project.h"

FreqList *nlp_word_frequency(TokenList *tokens)
{
    if (tokens == NULL)
        return NULL;

    if (tokens->count > 0 && tokens->tokens == NULL)
        return NULL;

    FreqList *freq_list = malloc(sizeof(FreqList));

    if (freq_list == NULL)
        return NULL;

    freq_list->capacity = tokens->count;
    freq_list->count = 0;
    freq_list->frequencies = NULL;

    if (tokens->count > 0) {
        freq_list->frequencies =
            malloc(tokens->count * sizeof(WordFreq));

        if (freq_list->frequencies == NULL) {
            free(freq_list);
            return NULL;
        }
    }

    for (size_t i = 0; i < tokens->count; i++) {
        if (tokens->tokens[i] == NULL) {
            free(freq_list->frequencies);
            free(freq_list);
            return NULL;
        }

        int found = 0;

        for (size_t j = 0; j < freq_list->count; j++) {
            if (strcmp(freq_list->frequencies[j].word,
                       tokens->tokens[i]) == 0) {
                freq_list->frequencies[j].count++;
                found = 1;
                break;
            }
        }

        if (!found) {
            freq_list->frequencies[freq_list->count].word =
                tokens->tokens[i];

            freq_list->frequencies[freq_list->count].count = 1;
            freq_list->count++;
        }
    }

    return freq_list;
}