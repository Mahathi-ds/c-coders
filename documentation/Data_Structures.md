# Data Structures

This document describes the custom data structures and types used in the NLP library, their fields, purpose, and memory-management requirements.

## 1. Text Processing Structures

### 1.1 `struct word`

Represents an individual token generated during word tokenization.

```c
struct word {
    char data[1000];
    int idx;
};
```

| Field | Type | Description |
|---|---|---|
| `data` | `char[1000]` | Stores the token as a null-terminated string. |
| `idx` | `int` | Tracks the current position while storing characters in the token. |

**Usage:**
- Used by `word_tokenize()` to store individual words, punctuation marks, and digits.
- The tokens are stored in the global `stack` array.
- The global variable `rear` keeps track of the number of tokens generated.



### 1.2 `Option`

Used to configure the behaviour of text normalization.

```c
typedef struct {
    int digit_removal;
    int punctuation_removal;
    int special_character_removal;
} Option;
```

| Field | Type | Description |
|---|---|---|
| `digit_removal` | `int` | Set to `1` to remove digits, `0` to keep them. |
| `punctuation_removal` | `int` | Set to `1` to remove punctuation marks, `0` to keep them. |
| `special_character_removal` | `int` | Set to `1` to remove non-printable characters, `0` to keep them. |

**Usage:**
- Passed to `text_normalization()` to specify which types of characters should be removed or processed.
- Allows the caller to customize normalization without changing the function implementation.




---

## 2. Token and Frequency Structures

### 2.1 `TokenList`

Stores a dynamically allocated collection of token strings.

```c
typedef struct {
    char **tokens;
    size_t count;
    size_t capacity;
} TokenList;
```

| Field | Type | Description |
|---|---|---|
| `tokens` | `char **` | Array of pointers to individual token strings. |
| `count` | `size_t` | Number of tokens currently stored. |
| `capacity` | `size_t` | Total number of token slots allocated. |

**Usage:**
- Used as input to functions such as `nlp_generate_ngrams()` and `nlp_word_frequency()`.
- Also represents the output of n-gram generation.


### 2.2 `WordFreq`

Stores a word and its frequency.

```c
typedef struct {
    char *word;
    int count;
} WordFreq;
```

| Field | Type | Description |
|---|---|---|
| `word` | `char *` | Pointer to the word whose frequency is being stored. |
| `count` | `int` | Number of occurrences of the word. |

**Usage:**
- Represents one unique word and its occurrence count.
- Used as an element inside `FreqList`.


### 2.3 `FreqList`

Stores the collection of unique words and their frequencies.

```c
typedef struct {
    WordFreq *frequencies;
    size_t count;
    size_t capacity;
} FreqList;
```

| Field | Type | Description |
|---|---|---|
| `frequencies` | `WordFreq *` | Dynamically allocated array of word-frequency entries. |
| `count` | `size_t` | Number of unique words stored. |
| `capacity` | `size_t` | Total number of allocated entries. |

**Usage:**
- Returned by `nlp_word_frequency()`.
- Stores each unique word along with its frequency.



---

## 3. Vectorization Structures

### 3.1 `Vocab`

Stores the unique words identified across a collection of documents.

```c
typedef struct {
    char **words;
    int size;
    int capacity;
} Vocab;
```

| Field | Type | Description |
|---|---|---|
| `words` | `char **` | Array of strings representing unique vocabulary words. |
| `size` | `int` | Number of unique words currently stored. |
| `capacity` | `int` | Total allocated capacity of the vocabulary array. |

**Usage:**
- Maintains the vocabulary used during vectorization.
- Each word is assigned a position in the vocabulary.
- The position of a word determines its corresponding column in the document vectors.

### 3.2 `VectorizerResult`

Stores the vectorized representation of a collection of documents along with the vocabulary.

```c
typedef struct {
    int **vectors;
    int num_docs;
    Vocab vocab;
} VectorizerResult;
```

| Field | Type | Description |
|---|---|---|
| `vectors` | `int **` | Two-dimensional array containing the vector representation of each document. |
| `num_docs` | `int` | Number of documents represented. |
| `vocab` | `Vocab` | Vocabulary used to construct the document vectors. |

**Usage:**
- Returned by the vectorizer function.
- Each row represents one document.
- Each column corresponds to a word in the vocabulary.
- Used by algorithms such as Naive Bayes and Jaccard similarity.



---

## 4. Naive Bayes Structures

The Naive Bayes implementation uses additional structures to organize class information, word frequencies, and probability calculations.

### 4.1 `prior`

Stores information about a class and its prior probability.

```c
typedef struct {
    char *label;
    int count;
    double prob;
} prior;
```

| Field | Type | Description |
|---|---|---|
| `label` | `char *` | Name of the class. |
| `count` | `int` | Number of training documents belonging to the class. |
| `prob` | `double` | Prior probability of the class. |



### 4.2 `ClassWordCounts`

Stores word-frequency information associated with a particular class.

```c
typedef struct {
    char *label;
    WordFreq *word_counts;
    int size;
} ClassWordCounts;
```

| Field | Type | Description |
|---|---|---|
| `label` | `char *` | Class label. |
| `word_counts` | `WordFreq *` | Array containing words and their counts for that class. |
| `size` | `int` | Number of word-count entries. |


### 4.3 `wordclasscounts`

Stores the class-wise occurrence information for a particular word.

```c
typedef struct {
    char *word;
    classcount *classes;
    int size;
} wordclasscounts;
```

| Field | Type | Description |
|---|---|---|
| `word` | `char *` | Word being considered. |
| `classes` | `classcount *` | Array containing class labels and their associated counts. |
| `size` | `int` | Number of class entries. |

### 4.4 `ClassDocuments`

Stores the indices of training documents belonging to a particular class.

```c
typedef struct {
    char *label;
    int *doc_indices;
    int count;
} ClassDocuments;
```

| Field | Type | Description |
|---|---|---|
| `label` | `char *` | Class label. |
| `doc_indices` | `int *` | Array containing indices of documents belonging to the class. |
| `count` | `int` | Number of documents in the class. |

### 4.5 `NaiveBayesModel`

Stores the learned information required to classify new documents.

The model contains the class information, class priors, vocabulary, and word likelihoods calculated during training.

**Usage:**
- Created and returned by `Naivebayes_fit()`.
- Passed to `Naivebayes_predict()` to classify new documents.
- Can be reused for multiple prediction calls without retraining.

**Memory considerations:**
- The model owns dynamically allocated memory.
- Use `NaiveBayes_free(&model)` to release the model's allocated memory.
- Do not use `free(model)` because the model itself is a structure variable, not a dynamically allocated pointer.
---