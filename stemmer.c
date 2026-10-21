#include <string.h>
#include "../include/project.h"

void nlp_stem_word(char *word) {
    if (!word) return;
    size_t len = strlen(word);
    
    /* Basic suffix stripping */
    if (len > 4 && strcmp(word + len - 3, "ing") == 0) {
        word[len - 3] = '\0';
    } else if (len > 3 && strcmp(word + len - 2, "ed") == 0) {
        word[len - 2] = '\0';
    } else if (len > 3 && strcmp(word + len - 2, "es") == 0) {
        word[len - 2] = '\0';
    } else if (len > 2 && word[len - 1] == 's') {
        word[len - 1] = '\0';
    }
}