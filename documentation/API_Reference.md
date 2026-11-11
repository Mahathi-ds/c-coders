# API Reference

This page is a quick lookup for every function in the library. It lists what each function does, what it needs, and what it gives back.

For explanations and examples, follow the links to [Text Processing](Text_processing.md) and [Algorithms](Algorithms.md). For the structures used below, see [Data Structures](Data_Structures.md).

All functions are declared in `project.h`:

```c
#include "project.h"
```

## 1. Constants and Global Variables

| Name | Meaning |
|---|---|
| `SENTENCE_SIZE` | Maximum length of one sentence stored by `sentence_split()`. |
| `DEFAULT` | Pass this as `max_sentences` to let `sentence_split()` choose the limit itself. |
| `stack` | Global array that holds the tokens made by `word_tokenize()`. |
| `rear` | Global number that tells you how many tokens are in `stack`. |

## 2. Text Processing Functions

| Function | What it does | Returns |
|---|---|---|
| `void text_normalization(char *text, Option opt)` | Cleans the text: removes extra spaces, and optionally digits, punctuation and special characters. | Nothing. Changes `text` directly. |
| `int sentence_split(char *text, char sentences[][SENTENCE_SIZE], int max_sentences)` | Splits text into sentences at `.`, `!` and `?`. | Number of sentences found, or `0` if the input is invalid. |
| `struct word *word_tokenize(char text[])` | Splits text into words, punctuation marks and single digits. | Pointer to the global `stack` array, or `NULL` if `text` is `NULL`. |
| `int make_lower(char text[])` | Converts text to lowercase. | `0` on success, `-1` if `text` is `NULL`. |
| `int make_upper(char text[])` | Converts text to uppercase. | `0` on success, `-1` if `text` is `NULL`. |
| `int stop_words(char stopwords[][100])` | Reads the stop words from `data/EN-Stopwords.txt`. | Number of stop words read, or `-1` if the file cannot be opened. |
| `int remove_stopwords(struct word *tokens, int keep_negations)` | Removes stop words from a token array. Use `keep_negations = 1` to keep `no`, `not` and `nor`. | `0` on success, `-1` on error. |
| `FreqList *nlp_word_frequency(TokenList *tokens)` | Counts how many times each unique word appears. | New `FreqList`, or `NULL` on error. |
| `TokenList *nlp_generate_ngrams(TokenList *tokens, int n)` | Joins `n` neighbouring tokens into n-grams (for example, bigrams when `n = 2`). | New `TokenList`, or `NULL` on error. |
| `void nlp_stem_word(char *word)` | Reduces a word to its stem (for example, `running` becomes `run`). | Nothing. Changes `word` directly. |

Details: [Text Processing](Text_processing.md)

## 3. Naive Bayes Functions

| Function | What it does | Returns |
|---|---|---|
| `NaiveBayesModel Naivebayes_fit(char **docs, char **labels, int num_docs)` | Trains a classifier from labelled documents. | A `NaiveBayesModel`. |
| `char **Naivebayes_predict(NaiveBayesModel model, char **test_docs, int num_docs)` | Predicts a label for each test document. | Array of label strings, or `NULL` on failure. |
| `void NaiveBayes_free(NaiveBayesModel *model)` | Frees the memory owned by the model. | Nothing. |

Details: [Algorithms](Algorithms.md)

## 4. Other Algorithms

The library also provides vectorisation, TF-IDF, cosine similarity, Jaccard similarity, sentiment scoring, keyword extraction and document similarity (see the [Overview](overview.md)).

> **To be added:** function prototypes, parameters and return values for these functions. Use the same table format as above.

## 5. Structures

| Structure | Used for |
|---|---|
| `struct word` | One token from `word_tokenize()`. |
| `Option` | Settings for `text_normalization()`. |
| `TokenList` | A list of token strings. |
| `WordFreq` | One word and its count. |
| `FreqList` | A list of `WordFreq` entries. |
| `Vocab` | The unique words found in a set of documents. |
| `VectorizerResult` | Document vectors together with their vocabulary. |
| `NaiveBayesModel` | A trained Naive Bayes classifier. |

Details: [Data Structures](Data_Structures.md)

## 6. Who Frees What?

| Function | Memory rule |
|---|---|
| `word_tokenize()` | Never free the result. It is a global array. |
| `nlp_word_frequency()` | Free the `FreqList` and its `frequencies` array. Never free the words inside. |
| `nlp_generate_ngrams()` | The caller must free the returned `TokenList`. |
| `Naivebayes_fit()` | Call `NaiveBayes_free(&model)`. Never use `free(model)`. |
| `Naivebayes_predict()` | Free each `predictions[i]`, then free `predictions`. |

## 7. Return Value Quick Guide

| Value | Usually means |
|---|---|
| `NULL` | Invalid input or memory could not be allocated. |
| `-1` | An error, such as a `NULL` pointer or a missing file. |
| `0` from `make_lower()`, `make_upper()`, `remove_stopwords()` | Success. |
| `0` from `sentence_split()` | No sentences were found, or the input was invalid. |

Always check the return value before using the result.