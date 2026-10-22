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

#define EXTRA_SPACE_REMOVAL_FALSE \
    (Option){1, 1, 1, 0}
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

#endif