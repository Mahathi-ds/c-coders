
# Data Structures

This document describes the custom data structures and types used in the NLP library, their fields, purpose, and memory-management requirements.

## 1\. Text Processing Structures

### 1.1 `struct word`

Represents an individual token generated during word tokenization.

c

```c
struct word {
    char data[1000];
    int idx;
};
```

| Field | Type | Description |
| --- | --- | --- |
| `data` | `char[1000]` | Stores the token as a null-terminated string. |
| `idx` | `int` | Tracks the current position while storing characters in the token. |

**Usage:**

- Used by `word_tokenize()` to store individual words, punctuation marks, and digits.
- The tokens are stored in the global `stack` array.
- The global variable `rear` keeps track of the number of tokens generated.

### 1.2 `Option`

Used to configure the behaviour of text normalization.

c

```c
typedef struct {
    int digit_removal;
    int punctuation_removal;
    int special_character_removal;
} Option;
```

| Field | Type | Description |
| --- | --- | --- |
| `digit_removal` | `int` | Set to `1` to remove digits, `0` to keep them. |
| `punctuation_removal` | `int` | Set to `1` to remove punctuation marks, `0` to keep them. |
| `special_character_removal` | `int` | Set to `1` to remove non-printable characters, `0` to keep them. |

**Usage:**

- Passed to `text_normalization()` to specify which types of characters should be removed or processed.
- Allows the caller to customize normalization without changing the function implementation.

---

## 2\. Token and Frequency Structures

### 2.1 `TokenList`

Stores a dynamically allocated collection of token strings.

c

```c
typedef struct {
    char **tokens;
    size_t count;
    size_t capacity;
} TokenList;
```

| Field | Type | Description |
| --- | --- | --- |
| `tokens` | `char **` | Array of pointers to individual token strings. |
| `count` | `size_t` | Number of tokens currently stored. |
| `capacity` | `size_t` | Total number of token slots allocated. |

**Usage:**

- Used as input to functions such as `nlp_generate_ngrams()` and `nlp_word_frequency()`.
- Also represents the output of N-gram generation.
- Used by `sentiment_analyze()` as the input for sentiment classification.
- Provides a common token representation for different NLP algorithms.

### 2.2 `WordFreq`

Stores a word and its frequency.

c

```c
typedef struct {
    char *word;
    int count;
} WordFreq;
```

| Field | Type | Description |
| --- | --- | --- |
| `word` | `char *` | Pointer to the word whose frequency is being stored. |
| `count` | `int` | Number of occurrences of the word. |

**Usage:**

- Represents one unique word and its occurrence count.
- Used as an element inside `FreqList`.
- Also used by Naive Bayes to maintain word-frequency information for individual classes.

### 2.3 `FreqList`

Stores the collection of unique words and their frequencies.

c

```c
typedef struct {
    WordFreq *frequencies;
    size_t count;
    size_t capacity;
} FreqList;
```

| Field | Type | Description |
| --- | --- | --- |
| `frequencies` | `WordFreq *` | Dynamically allocated array of word-frequency entries. |
| `count` | `size_t` | Number of unique words stored. |
| `capacity` | `size_t` | Total number of allocated entries. |

**Usage:**

- Returned by `nlp_word_frequency()`.
- Stores each unique word along with its frequency.
- Used internally by keyword extraction to identify frequently occurring words.
- Provides frequency information for text analysis.

---

## 3\. Keyword Extraction Structures

The Keyword Extraction algorithm identifies important words in a document based on their occurrence frequencies.

It uses the Word Frequency algorithm to calculate the number of occurrences of each unique word and selects the most frequent words as keywords.

### 3.1 `Keyword`

Stores an individual extracted keyword and its frequency.

c

```c
typedef struct {
    char *word;
    size_t frequency;
} Keyword;
```

| Field | Type | Description |
| --- | --- | --- |
| `word` | `char *` | Dynamically allocated string containing the extracted keyword. |
| `frequency` | `size_t` | Number of times the keyword appears in the input document. |

**Usage:**

- Represents one extracted keyword.
- Stores the keyword together with its occurrence frequency.
- Used as an element inside `KeywordList`.
- Keywords are arranged according to descending frequency.

### 3.2 `KeywordList`

Stores the collection of extracted keywords.

c

```c
typedef struct {
    Keyword *keywords;
    size_t count;
    size_t capacity;
} KeywordList;
```

| Field | Type | Description |
| --- | --- | --- |
| `keywords` | `Keyword *` | Dynamically allocated array of `Keyword` structures. |
| `count` | `size_t` | Number of extracted keywords currently stored. |
| `capacity` | `size_t` | Total allocated capacity of the keyword array. |

**Usage:**

- Used as the output structure of `nlp_extract_keywords()`.
- Stores extracted keywords and their corresponding frequencies.
- Supports limiting the number of extracted keywords using `max_keywords`.
- Keywords are sorted by descending frequency.
- Alphabetical ordering is used when two keywords have equal frequencies.

**Memory considerations:**

- The keyword array is dynamically allocated.
- Each keyword string is separately allocated.
- Use `nlp_keyword_list_free()` to release the keyword strings and array.
- Do not free the individual keyword strings separately when using the cleanup function.

---

## 4\. Sentiment Analysis Structures

The Sentiment Analysis algorithm identifies the emotional polarity of text using positive and negative word dictionaries.

It also considers linguistic modifiers such as negations, intensifiers, diminishers and contrast words to improve sentiment scoring.

### 4.1 `SentimentResult`

Stores the output of sentiment analysis.

c

```c
typedef struct {
    int positive_count;
    int negative_count;
    double score;
    Sentiment sentiment;
} SentimentResult;
```

| Field | Type | Description |
| --- | --- | --- |
| `positive_count` | `int` | Number of positively classified sentiment words. |
| `negative_count` | `int` | Number of negatively classified sentiment words. |
| `score` | `double` | Normalized sentiment score calculated from the sentiment words and their modifiers. |
| `sentiment` | `Sentiment` | Final sentiment classification: positive, negative or neutral. |

**Usage:**

- Used as the output structure of `sentiment_analyze()`.
- Stores the number of positive and negative sentiment words.
- Stores the normalized sentiment score.
- Provides the final sentiment classification.

### 4.2 `Sentiment`

Represents the possible sentiment classifications.

The implementation uses the following sentiment constants:

c

```c
SENTIMENT_POSITIVE
SENTIMENT_NEGATIVE
SENTIMENT_NEUTRAL
```

| Constant | Meaning |
| --- | --- |
| `SENTIMENT_POSITIVE` | The input text expresses positive sentiment. |
| `SENTIMENT_NEGATIVE` | The input text expresses negative sentiment. |
| `SENTIMENT_NEUTRAL` | The input text expresses neutral sentiment or has a score within the neutral threshold. |

**Usage:**

- Used by the `sentiment` field in `SentimentResult`.
- Represents the final classification produced by sentiment analysis.
- The classification is determined using the normalized sentiment score.

### 4.3 Internal Sentiment Dictionary

The sentiment implementation uses an internal dictionary structure to store positive and negative words.

c

```c
typedef struct {
    char **words;
    size_t count;
    size_t capacity;
} Dictionary;
```

| Field | Type | Description |
| --- | --- | --- |
| `words` | `char **` | Dynamically allocated array of dictionary word strings. |
| `count` | `size_t` | Number of words currently stored in the dictionary. |
| `capacity` | `size_t` | Total allocated capacity of the dictionary array. |

**Usage:**

- Maintains the positive and negative sentiment dictionaries.
- Positive and negative dictionaries are maintained separately.
- Supports loading dictionary words from files or accepting custom word lists.
- Dictionary words are sorted alphabetically.
- Binary search is used to check whether a word belongs to a sentiment dictionary.

**Memory considerations:**

- Each dictionary word is dynamically allocated.
- Custom dictionary words are copied from the caller's input.
- The original custom word arrays and strings remain owned by the caller.
- Internal dictionary memory is automatically released at program termination using `atexit()`.

**Note:** `Dictionary` is an internal implementation structure in the supplied sentiment source file. It is not necessarily a public structure available to applications through `project.h`.

---

## 5\. Vectorization Structures

### 5.1 `Vocab`

Stores the unique words identified across a collection of documents.

c

```c
typedef struct {
    char **words;
    int size;
    int capacity;
} Vocab;
```

| Field | Type | Description |
| --- | --- | --- |
| `words` | `char **` | Array of strings representing unique vocabulary words. |
| `size` | `int` | Number of unique words currently stored. |
| `capacity` | `int` | Total allocated capacity of the vocabulary array. |

**Usage:**

- Maintains the vocabulary used during vectorization.
- Each word is assigned a position in the vocabulary.
- The position of a word determines its corresponding column in the document vectors.

### 5.2 `VectorizerResult`

Stores the vectorized representation of a collection of documents along with the vocabulary.

c

```c
typedef struct {
    int **vectors;
    int num_docs;
    Vocab vocab;
} VectorizerResult;
```

| Field | Type | Description |
| --- | --- | --- |
| `vectors` | `int **` | Two-dimensional array containing the vector representation of each document. |
| `num_docs` | `int` | Number of documents represented. |
| `vocab` | `Vocab` | Vocabulary used to construct the document vectors. |

**Usage:**

- Returned by the vectorizer function.
- Each row represents one document.
- Each column corresponds to a word in the vocabulary.
- Used by algorithms such as Naive Bayes and Jaccard similarity.

---

## 6\. Naive Bayes Structures

The Naive Bayes implementation uses additional structures to organize class information, word frequencies, and probability calculations.

### 6.1 `prior`

Stores information about a class and its prior probability.

c

```c
typedef struct {
    char *label;
    int count;
    double prob;
} prior;
```

| Field | Type | Description |
| --- | --- | --- |
| `label` | `char *` | Name of the class. |
| `count` | `int` | Number of training documents belonging to the class. |
| `prob` | `double` | Prior probability of the class. |

### 6.2 `ClassWordCounts`

Stores word-frequency information associated with a particular class.

c

```c
typedef struct {
    char *label;
    WordFreq *word_counts;
    int size;
} ClassWordCounts;
```

| Field | Type | Description |
| --- | --- | --- |
| `label` | `char *` | Class label. |
| `word_counts` | `WordFreq *` | Array containing words and their counts for that class. |
| `size` | `int` | Number of word-count entries. |

### 6.3 `wordclasscounts`

Stores the class-wise occurrence information for a particular word.

c

```c
typedef struct {
    char *word;
    classcount *classes;
    int size;
} wordclasscounts;
```

| Field | Type | Description |
| --- | --- | --- |
| `word` | `char *` | Word being considered. |
| `classes` | `classcount *` | Array containing class labels and their associated counts. |
| `size` | `int` | Number of class entries. |

### 6.4 `ClassDocuments`

Stores the indices of training documents belonging to a particular class.

c

```c
typedef struct {
    char *label;
    int *doc_indices;
    int count;
} ClassDocuments;
```

| Field | Type | Description |
| --- | --- | --- |
| `label` | `char *` | Class label. |
| `doc_indices` | `int *` | Array containing indices of documents belonging to the class. |
| `count` | `int` | Number of documents in the class. |

### 6.5 `NaiveBayesModel`

Stores the learned information required to classify new documents.

The model contains the class information, class priors, vocabulary, and word likelihoods calculated during training.

**Usage:**

- Created and returned by `Naivebayes_fit()`.
- Passed to `Naivebayes_predict()` to classify new documents.
- Can be reused for multiple prediction calls without retraining.
- Uses the Multinomial Naive Bayes approach with Laplace smoothing.

**Memory considerations:**

- The model owns dynamically allocated memory.
- Use `NaiveBayes_free(&model)` to release the model's allocated memory.
- Do not use `free(model)` because the model itself is a structure variable, not a dynamically allocated pointer.
- Some internal pointers may refer to memory owned by other parts of the model and must not be freed separately.

---

## 7\. Memory Management Summary

The following table summarizes the ownership and memory-management requirements of the main structures.

| Structure | Memory management |
| --- | --- |
| `struct word` | Tokens are stored in the global token array. Do not free the global array. |
| `Option` | Usually allocated and managed by the caller. |
| `TokenList` | Dynamically allocated lists can be released using `nlp_token_list_free()`. |
| `WordFreq` | Its `word` pointer refers to an existing input string in `nlp_word_frequency()`. Do not free the borrowed string. |
| `FreqList` | Free its `frequencies` array followed by the `FreqList` structure. |
| `Keyword` | Its `word` string is dynamically allocated by keyword extraction. |
| `KeywordList` | Use `nlp_keyword_list_free()` to release its allocated memory. |
| `SentimentResult` | Supplied and managed by the caller. No separate allocation is performed by `sentiment_analyze()`. |
| `Dictionary` | Internal dictionary memory is automatically released at program termination. |
| `Vocab` | Follow the ownership rules of the vectorization implementation. |
| `VectorizerResult` | Follow the vectorizer's cleanup rules for vectors and vocabulary. |
| `prior` | Follow the ownership rules of the Naive Bayes model. |
| `ClassWordCounts` | Follow the ownership rules of the Naive Bayes model. |
| `wordclasscounts` | Follow the ownership rules of the Naive Bayes model. |
| `ClassDocuments` | Follow the ownership rules of the Naive Bayes model. |
| `NaiveBayesModel` | Use `NaiveBayes_free(&model)` to release all model-owned memory. |

Always ensure that dynamically allocated memory is released exactly once and that borrowed pointers are not freed independently.

For function prototypes and detailed usage instructions, refer to API Reference.

