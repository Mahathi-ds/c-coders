
# Text Processing

## 1. Introduction

Text processing is an essential step in Natural Language Processing (NLP), as raw textual data often contains inconsistencies, unnecessary characters, and structures that need to be handled before further analysis.

This module provides a collection of text-processing functions that transform raw text into a structured representation suitable for NLP algorithms.

The library implements the following text-processing functionalities:

- Text normalisation.
- Sentence splitting.
- Word tokenisation.
- Case normalisation.
- Stop-word removal.
- Word frequency analysis.
- N-gram generation.
- Basic stemming.

Each functionality is implemented as a separate operation, allowing users to invoke them independently or combine them into a complete text-processing pipeline.

---

## 2. Text Normalisation

### 2.1 Description

Text normalisation cleans the input text by removing unwanted characters and extra whitespace. The operations are controlled through an `Option` structure, allowing users to selectively enable digit, punctuation, and special character removal.

### 2.2 Working Principle

The function `text_normalization()` performs the following operations:

1. Removes extra spaces, tabs, and newline characters, replacing consecutive whitespace with a single space.
2. Removes digits if `digit_removal` is enabled.
3. Removes punctuation marks if `punctuation_removal` is enabled.
4. Removes non-printable characters if `special_character_removal` is enabled.
5. Removes leading and trailing whitespace.

Whitespace removal is always performed, irrespective of the options selected.

### 2.3 Function Signature

```c
void text_normalization(char *text, Option opt);
```

### 2.4 Example

**Input:**
```text
"Hello!!!   NLP 123\nWorld."
```

**Options:**
- `digit_removal = 1`
- `punctuation_removal = 1`
- `special_character_removal = 1`

**Output:**
```text
"Hello NLP World"
```

### 2.5 Implementation Details

- Performs in-place modification of the input string.
- Uses standard C character-handling functions from `ctype.h`.
- Returns immediately if the input pointer is `NULL`.
- Does not allocate additional memory.

---


## 2. Sentence Splitting

### Description

Sentence splitting divides a given text into individual sentences. In this implementation, sentences are identified using full stops (`.`), exclamation marks (`!`), and question marks (`?`). The extracted sentences are stored in a two-dimensional character array.

### Function Prototype

```c
int sentence_split(char *text, char sentences[][SENTENCE_SIZE],
                   int max_sentences);
```

### Parameters

- `text`: The input null-terminated string to be divided into sentences.
- `sentences`: A two-dimensional character array where the extracted sentences are stored.
- `max_sentences`: Maximum number of sentences to extract. If set to `DEFAULT`, the function uses the length of the input text as the maximum limit.

### Return Value

- Returns the number of sentences successfully extracted.
- Returns `0` if the input pointers are `NULL` or `max_sentences` is non-positive.

### Usage

Call `sentence_split()` by passing the input text, the array to store the sentences, and the maximum number of sentences.

```c
char sentences[100][SENTENCE_SIZE];

int count = sentence_split(text, sentences, 100);

for (int i = 0; i < count; i++)
{
    printf("%s\n", sentences[i]);
}
```

To use the default maximum limit:

```c
int count = sentence_split(text, sentences, DEFAULT);
```

### Example

**Input:**

```text
Hello everyone! Welcome to NLP. How are you?
```

**Output:**

```text
Hello everyone!
Welcome to NLP.
How are you?
```


---


## 3. Word Tokenization

### Description

Word tokenization splits the input text into individual tokens. In this implementation, words, punctuation marks, and digits are separated into individual tokens. The generated tokens are stored in a global array named `stack`.

### Function Prototype

```c
struct word *word_tokenize(char text[]);
```

### Parameters

- `text`: The input null-terminated string to be tokenized.

### Return Value

- Returns a pointer to the global `stack` array containing the generated tokens.
- Returns `NULL` if the input text is `NULL`.

The global variable `rear` stores the number of tokens generated.

### Usage

Call `word_tokenize()` by passing the input string. Use the global variable `rear` to determine the number of tokens generated.

```c
struct word *tokens;

tokens = word_tokenize(text);

if (tokens != NULL)
{
    for (int i = 0; i < rear; i++)
    {
        printf("%s\n", tokens[i].data);
    }
}
```

**Note:** The returned pointer refers to a global array. Do not free it. Each new call to `word_tokenize()` overwrites the previous token data, so copy the tokens if they need to be retained.

### Example

**Input:**

```text
Hello123, world!
```

**Output:**

```text
Hello
1
2
3
,
world
!
```



---


## 4. Case Normalisation

### Description

Case normalisation converts text into either lowercase or uppercase. This implementation provides two functions, `make_lower()` and `make_upper()`, which modify the input string directly.

### Function Prototypes

```c
int make_lower(char text[]);
int make_upper(char text[]);
```

### Parameters

- `text`: The input null-terminated string whose case needs to be changed.

### Return Value

- Returns `0` if the operation is successful.
- Returns `-1` if the input string is `NULL`.

### Usage

To convert a string into lowercase:

```c
char text[] = "Hello WORLD";
make_lower(text);
printf("%s", text);
```

To convert a string into uppercase:

```c
char text[] = "Hello WORLD";
make_upper(text);
printf("%s", text);
```

### Example

**Input:**

```text
Hello WORLD
```

**Output after make_lower():**

```text
hello world
```

**Output after make_upper():**

```text
HELLO WORLD
```


---


## 6. Stop Word Removal

### Description

Stop word removal eliminates commonly occurring words from a tokenized text. This implementation reads stop words from the file `data/EN-Stopwords.txt` and removes matching tokens from the input token array.

It also provides an option to preserve negation words (`no`, `not`, and `nor`) during removal.

### Function Prototypes

```c
int stop_words(char stopwords[][100]);
int remove_stopwords(struct word *tokens, int keep_negations);
```

### Parameters

**For `stop_words()`:**

- `stopwords`: A two-dimensional character array in which the stop words read from the file are stored.

**For `remove_stopwords()`:**

- `tokens`: Pointer to the token array from which stop words need to be removed.
- `keep_negations`: Controls whether negation words should be retained.
  - `1`: Preserves `no`, `not`, and `nor`.
  - `0`: Allows these words to be removed if they are present in the stop-word list.

### Return Value

**For `stop_words()`:**

- Returns the number of stop words successfully read from the file.
- Returns `-1` if the stop-word file cannot be opened.

**For `remove_stopwords()`:**

- Returns `0` if stop-word removal is successful.
- Returns `-1` if the token pointer is `NULL`, no tokens are available, or the stop-word file cannot be opened.

### Usage

Call `remove_stopwords()` after tokenizing the input text. Pass the token array and specify whether negation words should be preserved.

```c
struct word *tokens;

tokens = word_tokenize(text);

if (tokens != NULL)
{
    remove_stopwords(tokens, 1);

    for (int i = 0; i < rear; i++)
    {
        printf("%s\n", tokens[i].data);
    }
}
```

### Example

**Input tokens:**

```text
this
is
not
a
good
book
```

**Output with `keep_negations = 1` (assuming the other listed words are present in the stop-word file):**

```text
not
good
book
```

## 8. Word Frequency

### Description

Word frequency counts the occurrences of each unique word in a given token list. It stores each distinct word along with its corresponding frequency in a `FreqList`.

### Function Prototype

```c
FreqList *nlp_word_frequency(TokenList *tokens);
```

### Parameters

- `tokens`: Pointer to the input `TokenList` containing the words whose frequencies need to be calculated.

### Return Value

- Returns a pointer to a newly allocated `FreqList` containing unique words and their frequencies.
- Returns `NULL` if the input is invalid or memory allocation fails.

### Usage

Pass the input token list to `nlp_word_frequency()`.

```c
FreqList *freq = nlp_word_frequency(&tokens);

if (freq != NULL)
{
    for (size_t i = 0; i < freq->count; i++)
    {
        printf("%s : %zu\n",
               freq->frequencies[i].word,
               freq->frequencies[i].count);
    }
}
```

### Example

**Input tokens:**

```text
apple
banana
apple
orange
banana
apple
```

**Output:**

```text
apple : 3
banana : 2
orange : 1
```


---


## 7. N-gram Generation

### Description

N-gram generation creates sequences of consecutive tokens from a given token list. The value of `n` determines the number of tokens in each sequence.

For example, a bigram contains two consecutive tokens, while a trigram contains three. The generated n-grams are stored in a new `TokenList`.

### Function Prototype

```c
TokenList *nlp_generate_ngrams(TokenList *tokens, int n);
```

### Parameters

- `tokens`: Pointer to the input `TokenList` containing the tokens.
- `n`: Number of consecutive tokens to be combined into each n-gram.

### Return Value

- Returns a pointer to a newly allocated `TokenList` containing the generated n-grams.
- Returns `NULL` if the input is invalid, `n` is non-positive, there are insufficient tokens, or memory allocation fails.

### Usage

Pass the input token list and the desired n-gram size.

```c
TokenList *ngrams = nlp_generate_ngrams(&tokens, 2);

if (ngrams != NULL)
{
    for (size_t i = 0; i < ngrams->count; i++)
    {
        printf("%s\n", ngrams->tokens[i]);
    }
}
```

### Example

**Input tokens:**

```text
I
love
natural
language
processing
```

**For n = 2 (Bigrams):**

```text
I love
love natural
natural language
language processing
```

**For n = 3 (Trigrams):**

```text
I love natural
love natural language
natural language processing
```

---


## 5. Stemming

### Description

Stemming reduces words to their root or stem form by removing or replacing common word endings. This implementation follows the Porter stemming algorithm and modifies the input word directly.

### Function Prototype

```c
void nlp_stem_word(char *word);
```

### Parameters

- `word`: The input null-terminated string to be stemmed.

### Return Value

- Does not return a value (`void`).
- If the input is `NULL` or has two or fewer characters, no stemming is performed.

### Stemming Rules

The implementation applies the following rules in sequence:

1. **Plural removal:** Converts plural forms into their singular forms, such as removing plural endings or simplifying them.
2. **Past tense and continuous forms:** Removes past-tense and `-ing` endings when the remaining word contains a vowel, followed by additional adjustments where necessary.
3. **Y to I conversion:** Changes a final `y` to `i` when the word contains another vowel and does not end in `ay`.
4. **Derivational suffix replacement:** Replaces longer word endings with shorter forms, such as changing endings that indicate a process or quality into simpler forms.
5. **Suffix removal:** Removes certain endings related to qualities, states, and grammatical forms when the word has sufficient stem length.
6. **Final adjustments:** Removes a final `e` or repeated `l` under specific conditions based on the word structure.

### Usage

Pass the word to `nlp_stem_word()`. The original string is modified directly.

```c
char word[] = "running";

nlp_stem_word(word);

printf("%s", word);
```

### Example

**Input:**

```text
running
studies
relational
```

**Output:**

```text
run
studi
relat
```

---

