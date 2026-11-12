
# API Reference

This page is a quick lookup for every function in the library. It lists what each function does, what it needs, and what it gives back.

For explanations and examples, follow the links to Text Processing and Algorithms. For the structures used below, see Data Structures.

All functions are declared in `project.h`:

c

```c
#include "project.h"
```

## 1\. Constants and Global Variables

| Name | Meaning |
| --- | --- |
| `SENTENCE_SIZE` | Maximum length of one sentence stored by `sentence_split()`. |
| `DEFAULT` | Pass this as `max_sentences` to let `sentence_split()` choose the limit itself. |
| `stack` | Global array that holds the tokens made by `word_tokenize()`. |
| `rear` | Global number that tells you how many tokens are in `stack`. |

## 2\. Text Processing Functions

| Function | What it does | Returns |
| --- | --- | --- |
| `void text_normalization(char *text, Option opt)` | Cleans the text: removes extra spaces, and optionally digits, punctuation and special characters. | Nothing. Changes `text` directly. |
| `int sentence_split(char *text, char sentences[][SENTENCE_SIZE], int max_sentences)` | Splits text into sentences at `.`, `!` and `?`. | Number of sentences found, or `0` if the input is invalid. |
| `struct word *word_tokenize(char text[])` | Splits text into words, punctuation marks and single digits. | Pointer to the global `stack` array, or `NULL` if `text` is `NULL`. |
| `int make_lower(char text[])` | Converts text to lowercase. | `0` on success, `-1` if `text` is `NULL`. |
| `int make_upper(char text[])` | Converts text to uppercase. | `0` on success, `-1` if `text` is `NULL`. |
| `int stop_words(char stopwords[][100])` | Reads the stop words from `data/EN-Stopwords.txt`. | Number of stop words read, or `-1` if the file cannot be opened. |
| `int remove_stopwords(struct word *tokens, int keep_negations)` | Removes stop words from a token array. Use `keep_negations = 1` to keep `no`, `not` and `nor`. | `0` on success, `-1` on error. |
| `FreqList *nlp_word_frequency(TokenList *tokens)` | Counts how many times each unique word appears. | New `FreqList`, or `NULL` on error. |
| `TokenList *nlp_generate_ngrams(TokenList *tokens, int n)` | Joins `n` neighbouring tokens into n-grams (for example, bigrams when `n = 2`). | New `TokenList`, or `NULL` on error. |
| `void nlp_stem_word(char *word)` | Reduces a word to its stem using the Porter Stemming Algorithm (for example, `running` becomes `run`). | Nothing. Changes `word` directly. |

Details: Text Processing

## 3\. Naive Bayes Functions

The Naive Bayes classifier is a supervised machine learning algorithm used for text classification. This implementation uses the Multinomial Naive Bayes approach with Laplace smoothing.

During training, it calculates class priors and word likelihoods and stores the learned information in a `NaiveBayesModel`.

| Function | What it does | Returns |
| --- | --- | --- |
| `NaiveBayesModel Naivebayes_fit(char **docs, char **labels, int num_docs)` | Trains a classifier from labelled documents. Calculates class priors, vocabulary and word likelihoods. | A `NaiveBayesModel`. |
| `char **Naivebayes_predict(NaiveBayesModel model, char **test_docs, int num_docs)` | Predicts a label for each test document using the trained model. | Array of separately allocated label strings, or `NULL` on failure. |
| `void NaiveBayes_free(NaiveBayesModel *model)` | Frees the memory owned by the model and resets its fields. | Nothing. |

### Input Parameters

**Naivebayes\_fit()**

- `docs`: Array of strings containing training documents (`char **`).
- `labels`: Array of corresponding class labels (`char **`).
- `num_docs`: Number of training documents (`int`).

Each training document must have a corresponding label at the same index.

**Naivebayes\_predict()**

- `model`: Previously trained Naive Bayes model (`NaiveBayesModel`).
- `test_docs`: Array of strings containing test documents (`char **`).
- `num_docs`: Number of test documents (`int`).

**NaiveBayes\_free()**

- `model`: Pointer to the trained model (`NaiveBayesModel *`).

### Output and Memory Management

- `Naivebayes_fit()` returns a trained model containing the vocabulary, class priors and learned word-frequency information.
- `Naivebayes_predict()` returns a dynamically allocated array containing separately allocated predicted labels.
- Free each prediction string using `free(predictions[i])`, followed by `free(predictions)`.
- Release the trained model using `NaiveBayes_free(&model)`.
- Do not use `free(model)` because the model is returned as a structure by value.
- The same trained model can be reused for multiple prediction operations.
- The original training and testing document arrays and strings remain owned by the caller.

Details: Algorithms

## 4\. Sentiment Analysis Functions

The Sentiment Analysis algorithm is a rule-based Natural Language Processing technique used to identify the emotional polarity of text.

It uses positive and negative word dictionaries and considers linguistic modifiers such as negations, intensifiers, diminishers and contrast words.

The final sentiment is classified as Positive, Negative or Neutral based on a normalized sentiment score.

| Function | What it does | Returns |
| --- | --- | --- |
| `int sentiment_set_word_lists(const char *positive_words[], size_t positive_count, const char *negative_words[], size_t negative_count)` | Sets custom positive and negative sentiment dictionaries. Copies the supplied words into dynamically allocated memory. | `0` on success, `-1` on invalid input or allocation failure. |
| `int sentiment_analyze(const TokenList *tokens, SentimentResult *result)` | Analyzes a tokenized document using sentiment dictionaries and linguistic modifiers. | `0` on success, `-1` on invalid input, `-2` if dictionary initialization fails. |

### Input Parameters

**sentiment\_set\_word\_lists()**

- `positive_words`: Array of strings containing positive sentiment words (`const char *[]`).
- `positive_count`: Number of positive words (`size_t`).
- `negative_words`: Array of strings containing negative sentiment words (`const char *[]`).
- `negative_count`: Number of negative words (`size_t`).

**sentiment\_analyze()**

- `tokens`: Pointer to a `TokenList` containing the input document's tokens (`const TokenList *`).
- `result`: Pointer to a `SentimentResult` structure that stores the analysis results (`SentimentResult *`).

### Output Types

The `SentimentResult` structure stores:

| Field | Meaning |
| --- | --- |
| `positive_count` | Number of positively classified sentiment words. |
| `negative_count` | Number of negatively classified sentiment words. |
| `score` | Normalized sentiment score. |
| `sentiment` | Final sentiment classification. |

Possible sentiment classifications:

- `SENTIMENT_POSITIVE`
- `SENTIMENT_NEGATIVE`
- `SENTIMENT_NEUTRAL`

### Memory Management and Important Notes

- Custom positive and negative dictionaries are copied into dynamically allocated memory.
- The caller retains ownership of the original custom word arrays and strings.
- If custom dictionaries are not initialized, the algorithm loads the default dictionaries from `data/positive_words.txt` and `data/negative_words.txt`.
- Dictionary words are sorted alphabetically and searched using binary search.
- Dictionary memory is automatically released at program exit using `atexit()`.
- The input `TokenList` is not freed by `sentiment_analyze()`.
- The caller supplies the `SentimentResult` structure; the function does not allocate it.
- Sentiment analysis uses negation, intensity, diminisher and contrast handling to adjust sentiment scores.

## 5\. Keyword Extraction Functions

The Keyword Extraction algorithm identifies frequently occurring words in a document.

It uses the Word Frequency algorithm to calculate word occurrences and extracts the most frequent words as keywords.

The results are sorted by descending frequency. Alphabetical ordering is used when two keywords have equal frequencies.

| Function | What it does | Returns |
| --- | --- | --- |
| `int nlp_extract_keywords(struct word *const tokens[], size_t token_count, size_t max_keywords, KeywordList *result)` | Extracts the most frequently occurring words from an array of word tokens. | `0` on success, `-1` on invalid input, `-2` on allocation or frequency calculation failure. |
| `void nlp_keyword_list_free(KeywordList *list)` | Releases the dynamically allocated keyword strings and keyword array. | Nothing. |

### Input Parameters

**nlp\_extract\_keywords()**

- `tokens`: Array of pointers to `struct word` containing the document tokens (`struct word *const []`).
- `token_count`: Number of input tokens (`size_t`).
- `max_keywords`: Maximum number of keywords to extract (`size_t`).
- `result`: Pointer to a `KeywordList` structure used to store the extracted keywords (`KeywordList *`).

If `max_keywords` is `0`, all unique words are extracted.

### Output Types

The `KeywordList` structure contains:

| Field | Meaning |
| --- | --- |
| `keywords` | Array of `Keyword` structures containing extracted words and frequencies. |
| `count` | Number of extracted keywords. |
| `capacity` | Allocated capacity of the keyword array. |

Each `Keyword` contains:

- `word`: Dynamically allocated string containing the keyword.
- `frequency`: Number of occurrences of that keyword.

### Memory Management and Important Notes

- The function dynamically allocates the keyword array and individual keyword strings.
- The original `struct word` tokens and their strings are not freed.
- Keyword extraction internally uses `nlp_word_frequency()`.
- Keywords are sorted by descending frequency.
- Alphabetical order is used when frequencies are equal.
- Use `nlp_keyword_list_free()` to release the extracted keyword memory.
- Do not free individual keyword strings separately when using `nlp_keyword_list_free()`.

## 6\. Word Frequency Analysis Functions

The Word Frequency Analysis algorithm calculates how frequently each unique word occurs in a tokenized document.

It stores each unique word and its corresponding occurrence count in a dynamically allocated `FreqList` structure.

| Function | What it does | Returns |
| --- | --- | --- |
| `FreqList *nlp_word_frequency(TokenList *tokens)` | Counts the occurrences of each unique word in a tokenized document. | New `FreqList`, or `NULL` on invalid input or allocation failure. |

### Input Parameters

**nlp\_word\_frequency()**

- `tokens`: Pointer to a `TokenList` containing the input document's tokens (`TokenList *`).

### Output Types

The `FreqList` structure contains:

| Field | Meaning |
| --- | --- |
| `frequencies` | Array of `WordFreq` structures. |
| `count` | Number of unique words. |
| `capacity` | Allocated capacity of the frequency array. |

Each `WordFreq` structure contains:

- `word`: Pointer to the original input token string.
- `count`: Number of occurrences of that word.

### Memory Management and Important Notes

- The function dynamically allocates the `FreqList` structure and its frequency array.
- The `word` fields point to the original input strings. They are not duplicated.
- The input `TokenList` and its strings remain owned by the caller.
- Word comparisons use `strcmp()`, so uppercase and lowercase words are treated as different words.
- Unique words are stored in the order of their first occurrence.
- To release the result, free `freq_list->frequencies` first, followed by `freq_list`.
- Never free the individual `word` pointers because they refer to the original input strings.

## 7\. N-Gram Generation Functions

The N-Gram Generation algorithm creates sequences of consecutive tokens from a tokenized document.

It supports unigrams, bigrams, trigrams and higher-order N-grams.

| Function | What it does | Returns |
| --- | --- | --- |
| `TokenList *nlp_generate_ngrams(TokenList *tokens, int n)` | Combines `n` consecutive tokens into individual N-gram strings. | New dynamically allocated `TokenList`, or `NULL` on failure. |
| `void nlp_token_list_free(TokenList *list)` | Releases a dynamically allocated token list and its strings. | Nothing. |

### Input Parameters

**nlp\_generate\_ngrams()**

- `tokens`: Pointer to a `TokenList` containing the original tokens (`TokenList *`).
- `n`: Number of consecutive tokens in each N-gram (`int`).

### Output Types

- Returns a pointer to a `TokenList` containing the generated N-grams.
- Each generated N-gram is stored as a separately allocated string.
- The number of generated N-grams is `tokens->count - n + 1`.

Examples:

| N-Gram Type | Value of `n` | Example |
| --- | --- | --- |
| Unigram | 1 | `natural` |
| Bigram | 2 | `natural language` |
| Trigram | 3 | `natural language processing` |

### Memory Management and Important Notes

- The function dynamically allocates the output `TokenList`, token pointer array and individual N-gram strings.
- The original input tokens are not modified or freed.
- The requested N-gram size must be greater than zero.
- The N-gram size cannot exceed the number of input tokens.
- Words within an N-gram are separated by a single space.
- Use `nlp_token_list_free()` to release the generated N-gram list.
- Do not use `nlp_token_list_free()` on stack-allocated token lists or lists containing borrowed strings.

## 8\. Stemming Functions

The Stemming algorithm reduces words to their stem forms by removing or replacing common suffixes.

This implementation uses the Porter Stemming Algorithm, which processes words through five sequential stemming steps.

It uses consonant-vowel patterns, word measures, double consonant detection and suffix replacement rules to reduce different forms of words to a common stem.

| Function | What it does | Returns |
| --- | --- | --- |
| `void nlp_stem_word(char *word)` | Applies the Porter Stemming Algorithm to reduce a word to its stem. Modifies the original word directly. | Nothing. |

### Input Parameters

**nlp\_stem\_word()**

- `word`: Mutable, null-terminated character string containing the word to be stemmed (`char *`).

### Output Types

- Modifies the original input string in place.
- Does not return a separate output string.
- Words containing two or fewer characters remain unchanged.
- Returns without processing if the input is `NULL` or exceeds the supported maximum length.

### Memory Management and Important Notes

- The algorithm modifies the supplied input string directly.
- No additional dynamic memory is allocated.
- The caller retains ownership of the input character array.
- The input must be writable. String literals must not be passed.
- The algorithm does not convert the entire input word to lowercase.
- It attempts to preserve uppercase characters in replaced suffixes.
- The algorithm performs five sequential stemming steps.
- Ensure the writable buffer has sufficient capacity for any suffix replacement.

## 9\. Other Algorithms

The library also provides additional Natural Language Processing and text-analysis capabilities as part of the overall project.

These include:

| Algorithm | What it does |
| --- | --- |
| Vectorization | Converts text documents into numerical feature representations. |
| TF-IDF | Calculates the importance of words based on their frequency in a document and across a document collection. |
| Cosine Similarity | Measures the similarity between numerical document vectors. |
| Jaccard Similarity | Measures similarity between sets of words or features. |
| Sentiment Scoring | Determines the polarity and strength of sentiment in text. |
| Keyword Extraction | Identifies frequently occurring keywords in documents. |
| Document Similarity | Compares documents to determine how similar their content is. |

These algorithms work alongside the text-processing and classification functions to provide a broader NLP toolkit.

For additional explanations, see the Overview and Algorithms.

## 10\. Structures

| Structure | Used for |
| --- | --- |
| `struct word` | One token from `word_tokenize()`. |
| `Option` | Settings for `text_normalization()`. |
| `TokenList` | A list of token strings. |
| `WordFreq` | One word and its count. |
| `FreqList` | A list of `WordFreq` entries. |
| `Keyword` | One extracted keyword and its frequency. |
| `KeywordList` | A list of extracted keywords. |
| `SentimentResult` | Stores sentiment counts, normalized score and final sentiment classification. |
| `Vocab` | The unique words found in a set of documents. |
| `VectorizerResult` | Document vectors together with their vocabulary. |
| `NaiveBayesModel` | A trained Naive Bayes classifier. |

Details: Data Structures

## 11\. Who Frees What?

| Function | Memory rule |
| --- | --- |
| `word_tokenize()` | Never free the result. It is a global array. |
| `nlp_word_frequency()` | Free the `FreqList` and its `frequencies` array. Never free the words inside. |
| `nlp_generate_ngrams()` | The caller must free the returned `TokenList` using `nlp_token_list_free()`. |
| `nlp_token_list_free()` | Frees the token strings, token array and dynamically allocated `TokenList` structure. |
| `nlp_extract_keywords()` | Use `nlp_keyword_list_free()` to release the keyword list and its strings. |
| `nlp_keyword_list_free()` | Frees individual keyword strings and the keyword array. |
| `sentiment_set_word_lists()` | Copies custom dictionary words. Original caller-owned arrays and strings must remain managed by the caller. Internal dictionaries are cleaned up at program exit. |
| `sentiment_analyze()` | Does not allocate or free the supplied token list or result structure. |
| `nlp_stem_word()` | Does not allocate or free memory. Modifies the supplied writable string directly. |
| `Naivebayes_fit()` | Call `NaiveBayes_free(&model)`. Never use `free(model)`. |
| `Naivebayes_predict()` | Free each `predictions[i]`, then free `predictions`. |

## 12\. Return Value Quick Guide

| Value | Usually means |
| --- | --- |
| `NULL` | Invalid input or memory could not be allocated. |
| `-1` | An error, such as a `NULL` pointer, invalid input or a missing file. |
| `-2` | A dictionary initialization, memory allocation or other operation-specific failure. |
| `0` from `make_lower()`, `make_upper()`, `remove_stopwords()` | Success. |
| `0` from `sentiment_set_word_lists()` | Custom sentiment dictionaries successfully initialized. |
| `0` from `sentiment_analyze()` | Sentiment analysis completed successfully. |
| `0` from `nlp_extract_keywords()` | Keyword extraction completed successfully. |
| `0` from `sentence_split()` | No sentences were found, or the input was invalid. |
| `0` from `nlp_stem_word()` | Not applicable; the function returns `void`. |
| Positive integer from `sentence_split()` | Number of sentences found. |
| Positive integer from `stop_words()` | Number of stop words read. |

Always check the return value before using the result.

**Note:** The functions listed in this API reference describe the supplied library interfaces. Additional algorithms such as vectorization, TF-IDF, cosine similarity, Jaccard similarity and document similarity are included in the project overview; their exact prototypes and memory-management rules should be documented once their implementations are available.

One important note: I kept the API reference as a standalone project document rather than merging it with your Algorithms or Text Processing documentation. This way, `API_Reference.md` remains a quick lookup page, while the other Markdown files can contain the detailed explanations and examples.
