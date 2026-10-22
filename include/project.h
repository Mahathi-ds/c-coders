
#ifndef PROJECT_H
#define PROJECT_H

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

void text_normalization(char *text, Option opt);

int sentence_split(char *text, char sentences[][500], int max_sentences);

int calculate_tfidf(const char *filename,const char *target_word,const char *filenames[],int document_count,double *score);
#endif
