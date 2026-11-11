
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

**Memory considerations:**
- The current implementation uses a statically allocated global array.
- The returned pointer refers to this global array and must not be freed.
- A subsequent call to `word_tokenize()` overwrites the previous token data.

### 1.2 `Option`

Used to configure the behaviour of text normalization.

```c
typedef struct {
    int ...;
    int ...;
    int ...;
} Option;
```

The structure contains three integer flags that control the normalization options.

**Usage:**
- Passed to `text_normalization()` to specify which types of characters should be removed or processed.
- Allows the caller to customize normalization without changing the function implementation.

**Memory considerations:**
- Contains only integer fields.
- Does not require dynamic memory allocation or explicit deallocation.

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

**Memory considerations:**
- The token array and its strings may be dynamically allocated.
- The caller must use the appropriate cleanup procedure to release the list and its contents.
- Do not assume that every `TokenList` has the same ownership behaviour; check how the particular function creates or returns it.

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

**Memory considerations:**
- In `nlp_word_frequency()`, the `word` pointer refers to the original token string.
- The word is not copied into a separately allocated string.
- The original token strings must remain valid while the frequency list is being used.
- Do not free `word` separately through this structure.

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

**Memory considerations:**
- The caller must free the dynamically allocated `frequencies` array and the `FreqList` structure.
- The strings referenced by `frequencies[i].word` belong to the original input tokens and must not be freed through the frequency list.

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

**Memory considerations:**
- The vocabulary array and its individual strings are dynamically allocated.
- The appropriate cleanup procedure must release both the array and its owned strings.

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

**Memory considerations:**
- The vector matrix and vocabulary may contain dynamically allocated memory.
- The appropriate cleanup procedure must release every allocated row, the outer matrix, and the vocabulary's owned memory.
- Do not free the vocabulary separately if it is already being released as part of the complete `VectorizerResult` cleanup.

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
| `word_counts` | `WordCount *` | Array containing words and their counts for that class. |
| `size` | `int` | Number of word-count entries. |

### 4.3 `classcount`

Stores the occurrence count of a class.

```c
typedef struct {
    char *class;
    int count;
} classcount;
```

| Field | Type | Description |
|---|---|---|
| `class` | `char *` | Class label. |
| `count` | `int` | Number of occurrences associated with the class. |

### 4.4 `wordclasscounts`

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

### 4.5 `ClassDocuments`

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

### 4.6 `NaiveBayesModel`

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

