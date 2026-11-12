

# Natural Language Processing (NLP) Library Documentation

## 1. Naive Bayes Classifier

### Description

The Naive Bayes classifier is a supervised machine learning algorithm used for text classification.

This implementation uses a Multinomial Naive Bayes approach with Laplace smoothing. During training, it calculates class priors and word likelihoods and stores the learned information in a `NaiveBayesModel`.

### Function Prototypes

c

```c
NaiveBayesModel Naivebayes_fit(
    char **docs,
    char **labels,
    int num_docs
);

char **Naivebayes_predict(
    NaiveBayesModel model,
    char **test_docs,
    int num_docs
);

void NaiveBayes_free(NaiveBayesModel *model);
```

### Input Types

Naivebayes\_fit()

- `docs`: Array of strings containing the training documents (`char **`).
- `labels`: Array of strings containing the corresponding class labels (`char **`).
- `num_docs`: Number of training documents (`int`).

Each document must have a corresponding label at the same index.

Naivebayes\_predict()

- `model`: Trained Naive Bayes model (`NaiveBayesModel`).
- `test_docs`: Array of strings containing the documents to classify (`char **`).
- `num_docs`: Number of test documents (`int`).

NaiveBayes\_free()

- `model`: Pointer to the trained model (`NaiveBayesModel *`) whose allocated memory must be released.

### Output Types

Naivebayes\_fit()

- Returns a `NaiveBayesModel` containing the vocabulary, class priors, and learned word-frequency information.

Naivebayes\_predict()

- Returns a `char **` containing the predicted class label for each test document.
- Each predicted label is stored as a separately allocated string.
- Returns `NULL` if prediction cannot be performed successfully.

NaiveBayes\_free()

- Returns `void`.
- Releases the dynamically allocated memory owned by the model and resets its fields.

### How to Use

1. Prepare an array of training documents and their corresponding labels.
2. Pass them to `Naivebayes_fit()` to train the classifier.
3. Pass the trained model and test documents to `Naivebayes_predict()`.
4. Access the predicted label for each test document.
5. Free the prediction strings and prediction array.
6. Once the model is no longer required, call `NaiveBayes_free()`.

### Example

c

```c
char *docs[] = {
    "I love this movie",
    "This movie is terrible",
    "Amazing acting and story",
    "Very boring movie"
};

char *labels[] = {
    "Positive",
    "Negative",
    "Positive",
    "Negative"
};

int num_docs = 4;

NaiveBayesModel model = Naivebayes_fit(
    docs, labels, num_docs
);

char *test_docs[] = {
    "Amazing movie",
    "Very boring story"
};

int num_test = 2;

char **predictions = Naivebayes_predict(
    model, test_docs, num_test
);

if (predictions != NULL) {
    for (int i = 0; i < num_test; i++) {
        printf("%s\n", predictions[i]);
    }

    // Free individual prediction strings
    for (int i = 0; i < num_test; i++) {
        free(predictions[i]);
    }

    // Free prediction array
    free(predictions);
}

// Free trained model
NaiveBayes_free(&model);
```

### Memory Management and Important Notes

- Training model: `Naivebayes_fit()` dynamically allocates memory for the vocabulary, class priors, and word-frequency information. Always call `NaiveBayes_free(&model)` when the model is no longer needed.
- Prediction output: `Naivebayes_predict()` allocates memory for the prediction array and each predicted label separately. Free every `predictions[i]` first, followed by `predictions`.
- Model ownership: The model owns its vocabulary, class labels, and other dynamically allocated structures. Some internal pointers refer to memory owned by other parts of the model. These borrowed pointers must not be freed separately.
- Do not free the model directly: The model is returned as a structure by value, not as a dynamically allocated pointer. Use `NaiveBayes_free(&model)` rather than `free(model)`.
- Model reuse: The same trained model can be used to classify multiple sets of test documents. You do not need to train the model again for every prediction.
- Multiple models: If you train another model, remember to free the previous model when it is no longer needed.
- Input arrays: The training and test document arrays and their strings are supplied by the caller. They are not released by `NaiveBayes_free()`.

## 2. Sentiment Analysis

### Description

The Sentiment Analysis algorithm is a rule-based Natural Language Processing technique used to identify the emotional polarity of text.

This implementation uses positive and negative word dictionaries to determine whether a document expresses Positive, Negative, or Neutral sentiment.

It incorporates several linguistic features to improve sentiment detection:

- Positive and Negative Dictionaries: Identify words associated with positive and negative emotions.
- Negation Handling: Detects negation words such as "not", "never", and "cannot" and reverses the polarity of sentiment words within the configured negation scope.
- Intensity Handling: Detects intensifiers such as "very", "extremely", and "incredibly" to increase sentiment strength.
- Diminisher Handling: Detects words such as "slightly", "mildly", and "somewhat" to reduce sentiment strength.
- Contrast Handling: Detects contrast words such as "but", "however", and "although".
- Sentence Boundary Detection: Resets modifier scopes when sentence-ending punctuation is encountered.
- Sentiment Scoring: Calculates a normalized sentiment score based on the weighted sentiment words.

The final classification is determined using the calculated sentiment score.

### Function Prototypes

c

```c
int sentiment_set_word_lists(
    const char *positive_words[],
    size_t positive_count,
    const char *negative_words[],
    size_t negative_count
);

int sentiment_analyze(
    const TokenList *tokens,
    SentimentResult *result
);
```

### Input Types

sentiment\_set\_word\_lists()

- `positive_words`: Array of strings containing positive sentiment words (`const char *[]`).
- `positive_count`: Number of positive words (`size_t`).
- `negative_words`: Array of strings containing negative sentiment words (`const char *[]`).
- `negative_count`: Number of negative words (`size_t`).

sentiment\_analyze()

- `tokens`: Pointer to a `TokenList` containing the tokenized document (`const TokenList *`).
- `result`: Pointer to a `SentimentResult` structure used to store the sentiment analysis results (`SentimentResult *`).

If custom dictionaries are not supplied, the algorithm loads its default dictionaries from:

- `data/positive_words.txt`
- `data/negative_words.txt`

### Output Types

sentiment\_set\_word\_lists()

- Returns `0` if the custom dictionaries are successfully initialized.
- Returns `-1` if invalid input or memory allocation failure occurs.

sentiment\_analyze()

Stores the results in the supplied `SentimentResult` structure.

The structure contains:

- `positive_count`: Number of positively classified sentiment words.
- `negative_count`: Number of negatively classified sentiment words.
- `score`: Normalized sentiment score.
- `sentiment`: Final sentiment classification.

Possible sentiment classifications:

- `SENTIMENT_POSITIVE`
- `SENTIMENT_NEGATIVE`
- `SENTIMENT_NEUTRAL`

Return values:

- `0`: Analysis completed successfully.
- `-1`: Invalid input or NULL token encountered.
- `-2`: Dictionary initialization failed.

### How to Use

1. Prepare a tokenized document.
2. Optionally provide custom positive and negative word dictionaries using `sentiment_set_word_lists()`.
3. Create a `SentimentResult` structure.
4. Pass the token list and result structure to `sentiment_analyze()`.
5. Access the sentiment score, positive count, negative count, and final classification.

### Example

c

```c
const char *positive_words[] = {
    "good",
    "excellent",
    "amazing",
    "love",
    "happy"
};

const char *negative_words[] = {
    "bad",
    "terrible",
    "boring",
    "hate",
    "sad"
};

sentiment_set_word_lists(
    positive_words, 5,
    negative_words, 5
);

/* Assume tokens contains the tokenized document */

SentimentResult result;

int status = sentiment_analyze(&tokens, &result);

if (status == 0) {

    printf("Positive Count: %d\n",
           result.positive_count);

    printf("Negative Count: %d\n",
           result.negative_count);

    printf("Sentiment Score: %.2f\n",
           result.score);

    switch (result.sentiment) {

        case SENTIMENT_POSITIVE:
            printf("Sentiment: Positive\n");
            break;

        case SENTIMENT_NEGATIVE:
            printf("Sentiment: Negative\n");
            break;

        case SENTIMENT_NEUTRAL:
            printf("Sentiment: Neutral\n");
            break;
    }
}
```

### Memory Management and Important Notes

- Dictionary Initialization: Sentiment dictionaries are initialized once and reused for subsequent analysis calls.
- Custom Dictionaries: `sentiment_set_word_lists()` copies supplied words into dynamically allocated memory. The caller retains ownership of the original arrays and strings.
- Default Dictionaries: If custom dictionaries are not initialized, the algorithm loads the positive and negative dictionaries from the configured text files.
- Automatic Cleanup: Dictionary memory is released automatically when the program exits using `atexit()`.
- Case Insensitivity: Dictionary comparisons are case-insensitive.
- Binary Search: Dictionaries are sorted alphabetically and searched using binary search.
- Negation Scope: The implementation defines a negation window of three, with the window reduced when sentiment words are processed.
- Intensity Scope: Intensifiers and diminishers modify the strength of subsequent sentiment words according to the configured intensity window.
- Intensity Limits: The intensity multiplier is restricted to a maximum of `3.0` and a minimum of `0.25`.
- Sentence Boundaries: Negation, intensity, and contrast scopes are reset at sentence-ending punctuation.
- Input Ownership: The input `TokenList` is not freed or modified by the sentiment analysis function.
- Result Ownership: The caller supplies the `SentimentResult` structure. No dynamic memory is allocated for the result.

## 3. N-Gram Generation

### Description

The N-Gram Generation algorithm is a Natural Language Processing technique used to generate sequences of consecutive words from a tokenized document.

An N-gram represents a sequence of `N` consecutive tokens. This implementation combines adjacent tokens into dynamically allocated strings.

It supports different types of N-grams:

- Unigram (N=1): Represents individual words.
- Bigram (N=2): Represents sequences of two consecutive words.
- Trigram (N=3): Represents sequences of three consecutive words.
- Higher-order N-grams: Represents sequences containing more consecutive words.

N-gram generation is useful for identifying word relationships, analyzing phrases, extracting meaningful expressions, and supporting text classification.

### Function Prototypes

c

```c
TokenList *nlp_generate_ngrams(
    TokenList *tokens,
    int n
);

void nlp_token_list_free(TokenList *list);
```

### Input Types

nlp\_generate\_ngrams()

- `tokens`: Pointer to a `TokenList` containing the input document's tokens (`TokenList *`).
- `n`: Number of consecutive tokens to combine into each N-gram (`int`).

### Output Types

nlp\_generate\_ngrams()

- Returns a pointer to a dynamically allocated `TokenList` containing the generated N-grams.
- Each N-gram is stored as a separately allocated string.
- Returns `NULL` if invalid input, memory allocation failure, or an invalid N-gram size occurs.

The number of generated N-grams is calculated using:

$$
N_{\text{output}} = N_{\text{tokens}} - n + 1
$$

nlp\_token\_list\_free()

- Releases all dynamically allocated N-gram strings.
- Releases the token pointer array.
- Releases the `TokenList` structure.

### How to Use

1. Prepare a tokenized document.
2. Specify the required N-gram size.
3. Pass the token list and N-gram size to `nlp_generate_ngrams()`.
4. Access the generated N-gram strings.
5. Release allocated memory using `nlp_token_list_free()`.

### Example

c

```c
char *words[] = {
    "natural",
    "language",
    "processing",
    "algorithm"
};

TokenList tokens;

tokens.tokens = words;
tokens.count = 4;
tokens.capacity = 4;

/* Generate bigrams */

TokenList *bigrams =
    nlp_generate_ngrams(&tokens, 2);

if (bigrams != NULL) {

    for (size_t i = 0; i < bigrams->count; i++) {
        printf("%s\n", bigrams->tokens[i]);
    }

    nlp_token_list_free(bigrams);
}
```

Expected Output

text

```text
natural language
language processing
processing algorithm
```

### Memory Management and Important Notes

- Dynamic Allocation: The function dynamically allocates memory for the output `TokenList`, token pointer array, and individual N-gram strings.
- Memory Release: Always call `nlp_token_list_free()` to release the generated N-grams.
- Input Ownership: The original input tokens are not modified or freed.
- N-Gram Size: The value of `n` must be greater than zero.
- Token Count: The requested N-gram size cannot exceed the number of input tokens.
- Sequential Generation: N-grams are generated using consecutive tokens without skipping words.
- Space Separation: Words within each generated N-gram are separated by a single space.
- Output Order: N-grams maintain the original order of the input document.

## 4. Keyword Extraction

### Description

The Keyword Extraction algorithm is a Natural Language Processing technique used to identify the most frequently occurring words in a document.

This implementation uses the existing Word Frequency algorithm to calculate the occurrence of each token and extracts the most frequent words as keywords.

The extracted keywords are stored along with their corresponding frequencies.

The algorithm supports limiting the number of keywords returned by the user.

Keywords are sorted in descending order of frequency. When two keywords have the same frequency, alphabetical ordering is used.

### Function Prototypes

c

```c
int nlp_extract_keywords(
    struct word *const tokens[],
    size_t token_count,
    size_t max_keywords,
    KeywordList *result
);

void nlp_keyword_list_free(KeywordList *list);
```

### Input Types

nlp\_extract\_keywords()

- `tokens`: Array of pointers to `struct word` containing the input document's tokens (`struct word *const []`).
- `token_count`: Number of input tokens (`size_t`).
- `max_keywords`: Maximum number of keywords to extract (`size_t`).
- `result`: Pointer to a `KeywordList` structure where extracted keywords and frequencies are stored (`KeywordList *`).

If `max_keywords` is set to `0`, the function extracts all unique words.

### Output Types

nlp\_extract\_keywords()

Returns an integer status code.

The `KeywordList` structure contains:

- `keywords`: Array of `Keyword` structures.
- `count`: Number of extracted keywords.
- `capacity`: Allocated capacity of the keyword array.

Each `Keyword` structure contains:

- `word`: Extracted keyword stored as a dynamically allocated string.
- `frequency`: Number of occurrences of the keyword in the input document.

Return values:

- `0`: Keyword extraction completed successfully.
- `-1`: Invalid input.
- `-2`: Memory allocation or frequency calculation failure.

nlp\_keyword\_list\_free()

Releases the dynamically allocated keyword strings and keyword array, then resets the structure fields.

### How to Use

1. Prepare an array of token structures.
2. Specify the number of tokens.
3. Set the maximum number of keywords to extract.
4. Create a `KeywordList` structure.
5. Pass the inputs to `nlp_extract_keywords()`.
6. Access the extracted keywords and their frequencies.
7. Release allocated memory using `nlp_keyword_list_free()`.

### Example

c

```c
/* Assume word1, word2, etc. are valid struct word objects */

struct word *tokens[] = {
    &word1,
    &word2,
    &word3,
    &word4,
    &word5
};

size_t token_count = 5;
size_t max_keywords = 3;

KeywordList result;

int status = nlp_extract_keywords(
    tokens,
    token_count,
    max_keywords,
    &result
);

if (status == 0) {

    for (size_t i = 0; i < result.count; i++) {

        printf(
            "Keyword: %s | Frequency: %zu\n",
            result.keywords[i].word,
            result.keywords[i].frequency
        );
    }

    nlp_keyword_list_free(&result);
}
```

### Memory Management and Important Notes

- Dynamic Allocation: The function dynamically allocates memory for the keyword array and individual keyword strings.
- Frequency Dependency: Keyword extraction internally uses `nlp_word_frequency()` to calculate word occurrences.
- Maximum Keywords: When `max_keywords` is greater than zero, only the specified maximum number of keywords is returned.
- Unlimited Extraction: When `max_keywords` is zero, all unique words are returned.
- Sorting: Keywords are sorted by descending frequency.
- Alphabetical Ordering: Keywords with equal frequencies are sorted alphabetically.
- Input Ownership: The original `struct word` tokens and their strings are not freed or modified.
- Memory Release: Always call `nlp_keyword_list_free()` after using the extracted keywords.
- Independent Strings: Every extracted keyword is stored in separately allocated memory.

## 5. Word Frequency Analysis

### Description

The Word Frequency Analysis algorithm is a Natural Language Processing technique used to calculate how frequently individual words occur in a document.

This implementation processes a tokenized document and counts the occurrences of each unique word.

It stores each unique word along with its corresponding frequency in a dynamically allocated `FreqList` structure.

The algorithm is useful for:

- Identifying frequently occurring words.
- Understanding document content.
- Supporting keyword extraction.
- Analyzing word distribution.
- Identifying repeated terms in text.

### Function Prototypes

c

```c
FreqList *nlp_word_frequency(TokenList *tokens);
```

### Input Types

nlp\_word\_frequency()

- `tokens`: Pointer to a `TokenList` containing the input document's tokens (`TokenList *`).

The token list contains:

- `tokens`: Array of strings representing individual words.
- `count`: Number of tokens.
- `capacity`: Allocated capacity of the token array.

### Output Types

nlp\_word\_frequency()

Returns a pointer to a dynamically allocated `FreqList` structure.

The `FreqList` contains:

- `frequencies`: Array of `WordFreq` structures.
- `count`: Number of unique words.
- `capacity`: Allocated capacity of the frequency array.

Each `WordFreq` structure contains:

- `word`: Pointer to the original token string.
- `count`: Number of occurrences of that word.

Returns `NULL` if invalid input or memory allocation failure occurs.

### How to Use

1. Prepare a tokenized document.
2. Create a `TokenList` structure.
3. Pass the token list to `nlp_word_frequency()`.
4. Access the unique words and their corresponding frequencies.
5. Release the allocated frequency array and list structure.

### Example

c

```c
char *words[] = {
    "machine",
    "learning",
    "machine",
    "learning",
    "algorithm"
};

TokenList tokens;

tokens.tokens = words;
tokens.count = 5;
tokens.capacity = 5;

FreqList *freq_list =
    nlp_word_frequency(&tokens);

if (freq_list != NULL) {

    for (size_t i = 0; i < freq_list->count; i++) {

        printf(
            "Word: %s | Frequency: %d\n",
            freq_list->frequencies[i].word,
            freq_list->frequencies[i].count
        );
    }

    free(freq_list->frequencies);
    free(freq_list);
}
```

Expected Output

text

```text
Word: machine | Frequency: 2
Word: learning | Frequency: 2
Word: algorithm | Frequency: 1
```

### Memory Management and Important Notes

- Dynamic Allocation: The function dynamically allocates memory for the `FreqList` structure and its frequency array.
- Original Word References: The `word` field in each `WordFreq` structure points to the original input token string.
- No String Duplication: The original token strings are not copied into newly allocated memory.
- Input Ownership: The input `TokenList` and its strings remain owned by the caller.
- Memory Release: Free the frequency array first, followed by the `FreqList` structure.
- Case Sensitivity: Word comparison uses `strcmp()`, so uppercase and lowercase words are treated as different words.
- Frequency Counting: Each occurrence of an existing word increments its frequency count.
- Output Order: Unique words are stored in the order in which they first appear in the input document.
- Empty Input: An empty token list produces a valid empty frequency list.

## 6. Stemming Algorithm

### Description

The Stemming algorithm is a Natural Language Processing technique used to reduce words to their root or stem form by removing or replacing common suffixes.

This implementation uses the Porter Stemming Algorithm, which applies a sequence of suffix-removal and word-reduction rules to transform different forms of a word into a common stem.

The implementation processes words through five main stemming steps:

- Step 1 – Plural and Past Tense Reduction: Removes plural endings and common past-tense or continuous verb suffixes, such as `sses`, `ies`, `ed`, and `ing`.
- Step 2 – Derivational Suffix Reduction: Replaces suffixes such as `ational`, `izer`, `fulness`, and `ization` with shorter forms.
- Step 3 – Further Suffix Reduction: Removes or replaces suffixes such as `icate`, `ative`, `alize`, `fulness`, and `ness`.
- Step 4 – Suffix Removal: Removes additional suffixes such as `ance`, `ence`, `able`, `ment`, `ous`, and `ive` when the word satisfies the required morphological measure.
- Step 5 – Final Reduction: Removes certain trailing `e` and repeated `l` characters according to the stemming rules.

The algorithm also uses consonant-vowel patterns, word measures, double consonant detection, and consonant-vowel-consonant patterns to determine whether suffixes can be removed.

Stemming reduces vocabulary variations and helps improve text preprocessing for classification, keyword extraction, and information retrieval.

### Function Prototypes

c

```c
void nlp_stem_word(char *word);
```

### Input Types

nlp\_stem\_word()

- `word`: Mutable character string containing the word to be stemmed (`char *`).

The input must be a writable, null-terminated string because the algorithm modifies the original word directly.

### Output Types

nlp\_stem\_word()

- Returns `void`.
- Modifies the supplied input string in place.
- The original word is transformed into its stemmed form.
- Does not allocate a separate output string.
- If the input is `NULL`, empty, or contains more than the supported maximum length, the function returns without processing.

### How to Use

1. Prepare a writable character string containing the word.
2. Pass the string to `nlp_stem_word()`.
3. The algorithm applies the five stemming steps.
4. Access the modified string to obtain the stemmed word.
5. No additional memory needs to be allocated or released by the stemming function.

### Example

c

```c
char word1[] = "playing";
char word2[] = "connected";
char word3[] = "studies";
char word4[] = "running";
char word5[] = "relational";

/* Apply stemming */

nlp_stem_word(word1);
nlp_stem_word(word2);
nlp_stem_word(word3);
nlp_stem_word(word4);
nlp_stem_word(word5);

/* Display stemmed words */

printf("%s\n", word1);
printf("%s\n", word2);
printf("%s\n", word3);
printf("%s\n", word4);
printf("%s\n", word5);
```

Expected Output

text

```text
play
connect
studi
run
relat
```

### Memory Management and Important Notes

- In-Place Processing: The algorithm modifies the original input string rather than creating a separate output string.
- No Dynamic Allocation: The stemming function does not allocate or release memory.
- Writable Input: The input must be a mutable character array. String literals must not be passed because modifying them causes undefined behavior.
- Case Preservation: The algorithm performs case-insensitive suffix matching without converting the entire input word to lowercase. It attempts to preserve uppercase characters in replaced suffixes.
- Short Words: Words containing two or fewer characters are returned unchanged.
- NULL Handling: If the supplied word is `NULL`, the function returns without processing.
- Maximum Length: Words longer than the supported signed integer index range are returned unchanged.
- Five Processing Steps: The algorithm applies five sequential stemming stages to reduce suffixes.
- Input Ownership: The caller retains ownership of the original character array.
- Output Ownership: The stemmed result remains in the same input buffer.

# 7. Complete NLP Library Function Reference

The following table summarizes the algorithms and their corresponding public functions.

| Algorithm | Function | Purpose |
| --- | --- | --- |
| Naive Bayes | `Naivebayes_fit()` | Train a text classifier |
| Naive Bayes | `Naivebayes_predict()` | Predict document categories |
| Naive Bayes | `NaiveBayes_free()` | Release trained model memory |
| Sentiment Analysis | `sentiment_analyze()` | Identify text sentiment |
| Sentiment Dictionary | `sentiment_set_word_lists()` | Configure custom sentiment dictionaries |
| N-Gram Generation | `nlp_generate_ngrams()` | Generate consecutive word sequences |
| N-Gram Memory Management | `nlp_token_list_free()` | Release generated token lists |
| Keyword Extraction | `nlp_extract_keywords()` | Extract frequent keywords |
| Keyword Memory Management | `nlp_keyword_list_free()` | Release keyword memory |
| Word Frequency | `nlp_word_frequency()` | Calculate word occurrence counts |
| Stemming | `nlp_stem_word()` | Reduce words to their stems |

## 8. Overall Library Capabilities

Your NLP library currently provides the following capabilities:

Machine Learning

Multinomial Naive Bayes text classification with Laplace smoothing.

Sentiment Analysis

Positive, negative, and neutral sentiment detection with linguistic modifiers.

N-Gram Processing

Unigram, bigram, trigram, and higher-order sequence generation.

Keyword Extraction

Extraction and ranking of frequent document keywords.

Word Frequency Analysis

Counting and analyzing unique word occurrences.

Stemming

Porter-based suffix reduction and word normalization.

## 9. Important Technical Notes Before Publishing

I have retained the behavior described by your source code. A few implementation details are worth checking:

1. Stemming: Your implementation follows the classic Porter stemming approach, but the shared global variables `b`, `k`, and `j` make it non-reentrant and unsafe for simultaneous calls from multiple threads.
2. Stemming: `set_to()` can write beyond the original input buffer if a replacement suffix is longer than the suffix being replaced. Ensure the input buffer has sufficient writable capacity.
3. Sentiment: `CONTRAST_WEIGHT` is currently `1.0`, so contrast words do not change the numerical weighting of sentiment.
4. Sentiment: Your intensity window is reduced after ordinary tokens, whereas the negation window is reduced only after sentiment words.
5. Word Frequency: The frequency algorithm uses `strcmp()`, making its word counting case-sensitive.
6. Keyword Extraction: The current implementation ranks words by frequency but does not independently remove stop words or calculate semantic importance.
7. Memory Management: Consider introducing a dedicated `nlp_word_frequency_free()` function to simplify frequency-list cleanup.

