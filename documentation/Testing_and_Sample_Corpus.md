# Testing and Sample Corpus

## 1. Introduction

This document describes the test cases used to check the functionality of the NLP library. The tests cover normal inputs, boundary cases, invalid inputs, empty inputs, and large inputs wherever applicable.

The tests are organised module-wise. Each section describes the inputs tested and the behaviour that was checked.

---

## 2. Module-wise Testing

### 2.1 Case Normalisation

**Test file:** `test_case_normalisation.c`

The case normalisation functions were tested to verify that text is correctly converted to lowercase or uppercase.

**Test cases:**

- Normal lowercase conversion of a mixed-case string.
- Normal uppercase conversion of a mixed-case string.
- Already lowercase and already uppercase inputs.
- Mixed alphanumeric text containing punctuation.
- Single-character input.
- Empty string.
- NULL input, checking whether the functions return an error status.
- Non-alphabetic input, which should remain unchanged.
- Large input containing 10,000 uppercase characters, checking whether all characters are converted correctly.

### 2.2 Text Normalisation

**Test file:** `test_text_normalisation.c`

The text normalisation function was tested with different combinations of characters and normalisation options.

**Test cases:**

- Normal text containing uppercase letters, punctuation, digits, and special characters.
- Input with digit removal disabled.
- Input with punctuation removal disabled.
- Empty string.
- Input containing only spaces.
- Input containing multiple consecutive spaces.
- Single-character input.
- Input containing only digits and punctuation.
- NULL input, checking whether the function handles it safely.
- Large input containing repeated combinations of letters, digits, and punctuation.

The tests also checked the output length and displayed portions of the normalised output for larger inputs.

### 2.3 Sentence Splitting

**Test file:** `test_sentence_splitting.c`

Sentence splitting was tested to check whether text is divided correctly using sentence-ending punctuation and whether the maximum sentence limit is respected.

**Test cases:**

- Normal paragraph containing multiple sentences.
- Empty input.
- NULL input.
- Maximum sentence count set to zero.
- Negative maximum sentence count.
- Input without sentence-ending punctuation.
- Single-character sentence.
- Sentences ending with different punctuation marks (`.`, `!`, and `?`).
- Input containing leading and multiple spaces.
- Large input containing 1,000 short sentences, with the maximum output count limited to 10.
- Sentence length of 499 characters with a 500-character buffer.
- Input using the default maximum sentence count.
- Sentence longer than the output buffer, checking how the function handles a 600-character sentence with a 500-character buffer.

The tests displayed the resulting sentence count and sentence contents for inspection.

### 2.4 Word Tokenisation

**Test file:** `test_tokenize.c`

The word tokenisation function was tested to check whether input text is divided into individual tokens.

**Test cases:**

- Normal input containing two words.
- Input containing punctuation.
- Input containing digits.
- Input containing multiple spaces between words.
- Empty input, checking whether an empty token list is returned.
- NULL input, checking whether it is handled safely.
- Single-character input.
- Large input containing 900 repeated words, checking the resulting token count.
- Input containing only punctuation.

The generated tokens and token counts were printed for inspection.

### 2.5 Stop-word Removal

**Test file:** `test_stop_words.c`

Stop-word removal was tested to check whether common stop words are removed while meaningful words are retained.

**Test cases:**

- Normal sentence containing both stop words and meaningful words.
- Input containing only stop words, checking whether the resulting token count is zero.
- Input containing no stop words, checking whether all tokens are retained.
- Empty input.
- Repeated stop words mixed with meaningful words.
- Large input containing 890 repetitions of a stop word and 10 repetitions of a meaningful word, checking whether only the meaningful tokens remain.

The tests used word tokenisation before applying stop-word removal.

### 2.6 Word Frequency

**Test file:** `test_frequency.c`

Word frequency calculation was tested to check whether unique words are identified and their occurrences are counted correctly.

**Test cases:**

- Normal input containing repeated words.
- Input in which all words are repeated.
- Input containing only unique words.
- Single-token input.
- Empty token list.
- Case-sensitive words such as `Apple`, `apple`, and `APPLE`.
- Words containing punctuation, checking whether they are treated as distinct tokens.
- Large input containing 1,000 tokens distributed among 10 distinct words, checking whether each word has a frequency of 100.
- NULL token-list pointer.

The tests compared the generated unique-word count and individual frequencies with the expected values. They also checked for unexpected words.

### 2.7 Stemming

**Test file:** `test_stemmer.c`

The stemmer was tested using words from different stages of the stemming rules to check whether suffixes are reduced correctly.

**Test cases:**

- Basic words that should remain unchanged, such as `cat`, `dog`, and `book`.
- Words ending in common plural suffixes, such as `caresses`, `ponies`, `ties`, and `cats`.
- Words ending in `-ing` and `-ed`, such as `running`, `agreed`, and `disabled`.
- Words used to test intermediate suffix transformations, such as `relational`, `conditional`, and `rational`.
- Words used to test later suffix transformations, such as `triplicate`, `formalize`, `electrical`, and `goodness`.
- Empty string.
- One-character and two-character words.
- Repeated short words.
- Long input containing 200 characters.
- NULL input, checking whether the function handles it safely.

The actual stems were compared with expected stems, and the number of passed and failed cases was reported.

### 2.8 N-gram Generation

**Test file:** `ng.c`

N-gram generation was tested to check whether consecutive groups of `n` tokens are generated correctly.

**Test cases:**

- Generation of unigrams, bigrams, and trigrams from a normal token list.
- Boundary case where `n` is equal to the number of input tokens.
- Invalid values of `n`, including zero, a negative value, and a value greater than the number of tokens.
- Empty token list.
- NULL token-list pointer.
- Large input containing 1,000 tokens, checking the generated bigram count and inspecting the first and last bigrams.

The test also included memory cleanup for the generated n-grams and dynamically allocated input tokens.

---

## 3. Machine Learning and Similarity Demo Testing

The following sections are reserved for the demo programs. Their test cases, expected results, and observed outputs will be added after the corresponding demo files are included.

### 3.1 Naive Bayes Classification

**Demo file:** To be added.

**Training corpus:** To be documented.

**Test cases:**

*To be added after reviewing the Naive Bayes demo.*

**Observed results:**

*To be added.*

### 3.2 Sentiment Analysis

**Demo file:** To be added.

**Test cases:**

*To be added after reviewing the sentiment analysis demo.*

**Observed results:**

*To be added.*

### 3.3 Document Similarity

**Demo file:** To be added.

**Test cases:**

*To be added after reviewing the document similarity demo.*

**Observed results:**

*To be added.*

---

## 4. Memory Management Testing

Memory management was considered in tests involving dynamically allocated inputs and outputs.

The following cleanup operations were included where applicable:

- Freeing dynamically allocated input strings and token arrays.
- Freeing generated n-gram strings and their containing lists.
- Freeing word-frequency result structures and their allocated arrays.
- Freeing dynamically allocated large test inputs.

The ownership of memory returned by each function must be considered when freeing its results. In particular, structures containing pointers to input strings should not free those strings unless they own the allocated memory.

---

## 5. Sample Corpus

The sample corpus used for the machine learning demonstrations will be documented here.

### 5.1 Naive Bayes Training Corpus

*Training folder structure and class-wise documents to be added.*

### 5.2 Naive Bayes Testing Corpus

*Testing documents and their expected class labels to be added.*

### 5.3 Sentiment Analysis Corpus

*Sample positive and negative documents to be added.*

### 5.4 Document Similarity Samples

*Sample document pairs and their expected similarity behaviour to be added.*

---

## 6. Summary

The module-level tests cover the core text-processing functionality of the library, including case normalisation, text normalisation, sentence splitting, tokenisation, stop-word removal, word frequency calculation, stemming, and n-gram generation.

The machine learning and similarity sections will be completed using the corresponding demo programs and their test results.