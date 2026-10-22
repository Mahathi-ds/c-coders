#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "project.h" 
static int is_word_character(char ch);
static int is_word_equal(const char word1[], const char word2[]);
static int process_file(const char *filename,
                        const char *target_word,
                        int *target_count,
                        int *total_count);
static int calculate_document_frequency(
    const char *filenames[],
    int document_count,
    const char *target_word,
    int *document_frequency);
static double calculate_tf(int target_count, int total_count);
static double calculate_idf(int document_count, int document_frequency);
int is_word_character(char ch)
{
    return isalnum((unsigned char)ch);
}
static int is_word_equal(const char word1[], const char word2[])
{
    char temp1[1000];
    char temp2[1000];
    int i = 0;

    if (word1 == NULL || word2 == NULL)
    {
        return 0;
    }

    while (word1[i] != '\0' && i < 999)
    {
        temp1[i] = word1[i];
        i++;
    }
    temp1[i] = '\0';

    if (word1[i] != '\0')
    {
        return 0;
    }

    i = 0;

    while (word2[i] != '\0' && i < 999)
    {
        temp2[i] = word2[i];
        i++;
    }
    temp2[i] = '\0';

    if (word2[i] != '\0')
    {
        return 0;
    }

    make_lower(temp1);
    make_lower(temp2);

    return strcmp(temp1, temp2) == 0;
}

int process_file(const char *filename,
                 const char *target_word,
                 int *target_count,
                 int *total_count)
{
    FILE *file;
    char word[1000];
    char ch;
    int word_length = 0;
    int too_long = 0;

    if (filename == NULL || target_word == NULL ||
        target_count == NULL || total_count == NULL)
    {
        return 0;
    }

    *target_count = 0;
    *total_count = 0;

    if (target_word[0] == '\0')
    {
        return 0;
    }

    file = fopen(filename, "r");

    if (file == NULL)
    {
        return 0;
    }

    while ((ch = (char)fgetc(file)) != EOF)
    {
        if (is_word_character(ch))
        {
            if (word_length < 999)
            {
                word[word_length] = ch;
                word_length++;
            }
            else
            {
                too_long = 1;
            }
        }
        else if (word_length > 0 || too_long)
        {
            if (!too_long)
            {
                word[word_length] = '\0';

                if (is_word_equal(word, target_word))
                {
                    (*target_count)++;
                }
            }

            (*total_count)++;

            word_length = 0;
            too_long = 0;
        }
    }

    if (word_length > 0 || too_long)
    {
        if (!too_long)
        {
            word[word_length] = '\0';

            if (is_word_equal(word, target_word))
            {
                (*target_count)++;
            }
        }

        (*total_count)++;
    }

    if (ferror(file))
    {
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int calculate_document_frequency(const char *filenames[],
                                 int document_count,
                                 const char *target_word,
                                 int *document_frequency)
{
    int i;
    int target_count;
    int total_count;

    if (filenames == NULL || target_word == NULL ||
        document_frequency == NULL || document_count <= 0 ||
        target_word[0] == '\0')
    {
        return 0;
    }

    *document_frequency = 0;

    for (i = 0; i < document_count; i++)
    {
        if (filenames[i] == NULL)
        {
            return 0;
        }

        if (!process_file(filenames[i], target_word,
                          &target_count, &total_count))
        {
            return 0;
        }

        if (target_count > 0)
        {
            (*document_frequency)++;
        }
    }

    return 1;
}

double calculate_tf(int target_count, int total_count)
{
    if (target_count < 0 || total_count <= 0 ||
        target_count > total_count)
    {
        return 0.0;
    }

    return (double)target_count / total_count;
}

double calculate_idf(int document_count, int document_frequency)
{
    if (document_count <= 0 || document_frequency <= 0 ||
        document_frequency > document_count)
    {
        return 0.0;
    }

    return log((double)document_count / document_frequency);
}

int calculate_tfidf(const char *filename,
                    const char *target_word,
                    const char *filenames[],
                    int document_count,
                    double *score)
{
    int target_count;
    int total_count;
    int document_frequency;
    double tf;
    double idf;

    if (score == NULL)
    {
        return 0;
    }

    *score = 0.0;

    if (filename == NULL || target_word == NULL ||
        filenames == NULL || document_count <= 0 ||
        target_word[0] == '\0')
    {
        return 0;
    }

    if (!process_file(filename, target_word,
                      &target_count, &total_count))
    {
        return 0;
    }

    if (!calculate_document_frequency(filenames, document_count,
                                      target_word,
                                      &document_frequency))
    {
        return 0;
    }

    tf = calculate_tf(target_count, total_count);
    idf = calculate_idf(document_count, document_frequency);

    *score = tf * idf;

    return 1;
}
