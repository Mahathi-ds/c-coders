# Limitations

This library is built to teach and demonstrate basic NLP in C. It is simple on purpose, so it has some limits. Knowing them will help you avoid surprises.

## 1. General Limitations

| **Limitation** | **What it means for you** |
|---|---|
| **English only** | The stop-word list, stemming rules and text handling are designed for English. Other languages may give poor results. |
| **Basic methods** | The library uses classic, simple techniques. It is not a replacement for modern machine-learning NLP tools. |
| **Not thread-safe** | Some functions use global variables (see section 3), so do not call them from several threads at the same time. |

## 2. Text Processing Limitations

### Text normalisation

- Changes the text directly. The original text is lost unless you keep a copy.
- Whitespace is always cleaned. This cannot be turned off.

### Sentence splitting

- Sentences end only at `.`, `!` and `?`.
- Abbreviations such as `Dr.` or `e.g.` may be wrongly treated as the end of a sentence.
- Decimal numbers such as `3.14` may be split incorrectly.
- Each sentence must fit within `SENTENCE_SIZE` characters.
- You must supply an array large enough for the sentences you expect.

### Word tokenisation

- Words like `well-known` may not be split the way you expect.
- Each token can hold at most 999 characters (the `data` field is 1000 characters).
- Tokens are stored in one global array, so a new call to `word_tokenize()` replaces the previous tokens. Copy tokens you want to keep.

### Stop-word removal

- The stop-word list is English only.
- Each stop word can hold at most 99 characters.

### Word frequency

- The frequency list points to the original token strings instead of copying them. If you free or change the tokens, the list becomes invalid.
- Words are counted exactly as written. `Apple` and `apple` count as different words unless you make the text lowercase first.
- The function uses nested loops to compare tokens with previously encountered words. Processing large token lists with many unique words can be slow.
- Memory is allocated based on the total number of input tokens, even if many tokens are repeated.

### N-gram generation

- Returns `NULL` if the input is `NULL`, `n` is zero or negative, or there are fewer tokens than `n`.
- Generates n-grams from consecutive tokens only. It does not consider the meaning or context of the words.
- Words within an n-gram are joined using a single space. Original spacing and formatting are not preserved.
- Punctuation is retained if it is present in the input tokens.
- Large token lists may require considerable memory because each generated n-gram is separately allocated.
- You must free the returned list using `nlp_token_list_free()` to avoid memory leaks.

### Stemming

- Follows the Porter stemming algorithm, which uses fixed rules. Some stems are not real words (for example, `studies` may become `studi`).
- Some different words may end up with the same stem, and some related words may not.
- Words with two or fewer characters are not changed.
- The input word is modified directly. Keep a copy if you need the original word.
- The algorithm does not convert the entire word to lowercase. Mixed uppercase and lowercase input may produce inconsistent results.
- The implementation uses global variables internally, so simultaneous calls from multiple threads are not thread-safe.

## 3. Algorithm Limitations

### Naive Bayes

- Treats every word as independent of the others, so word order and context are ignored.
- Needs labelled training data. Very small training sets give unreliable predictions.
- Can only predict labels it saw during training.
- Words that were not in the training data are not helpful for prediction.
- You must free the model and the predictions yourself. See [Algorithms](Algorithms.md).

### Sentiment analysis

- Uses predefined positive and negative word dictionaries. Words missing from these dictionaries do not contribute to the sentiment score.
- Calculates sentiment using individual word scores. It does not understand the complete meaning or context of a sentence.
- Sarcasm, irony, humour and implied meanings are not detected.
- Negation handling uses a predefined list of negation words and a limited scope. Complex sentence structures may produce incorrect results.
- Intensifiers and diminishers use fixed weights. The actual strength of an expression may differ depending on its context.
- Contrast words such as `but` and `however` apply a fixed weight to subsequent sentiment words. More complex contrast structures may not be handled correctly.
- Words appearing in both positive and negative dictionaries are ignored.
- The final sentiment is classified using fixed score thresholds. Mixed or subjective opinions may not be classified accurately.

### Keyword extraction

- Keywords are selected based on word frequency only. The algorithm does not consider the meaning or context of words.
- Common words may appear as keywords if they occur frequently. Stop-word removal must be performed separately if you want to exclude them.
- Words are compared exactly, so uppercase and lowercase versions are treated as different keywords unless the input is normalised beforehand.
- The function considers individual tokens only. It does not extract meaningful multi-word phrases.
- The maximum number of keywords is controlled by `max_keywords`. If it is `0`, all unique keywords are returned.
- Keywords with equal frequencies are sorted alphabetically, not by their importance or relevance.
- The function allocates separate memory for the returned keyword strings. You must free the keyword list using `nlp_keyword_list_free()`.
### Frequency
- The frequency list stores pointers to the original token strings rather than making copies. The original tokens must remain valid while the frequency list is being used.
- The function uses nested loops to compare each token with previously encountered words. Processing large token lists with many unique words can be slow.
- The function allocates memory based on the total number of input tokens, even when many tokens are repeated.
### Stemming

- Uses predefined suffix-removal rules based on the Porter stemming algorithm. It does not understand the meaning of words.
- Stemming may produce results that are not valid English words. For example, `studies` may become `studi`.
- Different words may produce the same stem.
- The input word is modified directly. Keep a copy if you need the original word.
- Words with two or fewer characters are not modified.
- The implementation uses fixed global variables internally, so simultaneous calls from multiple threads are not thread-safe.
### N-gram generation

- Generates n-grams from consecutive tokens only. It does not identify semantic relationships between words.
- Returns `NULL` if `n` is zero, negative, or greater than the number of input tokens.
- The generated n-grams contain words joined by a single space. Original spacing and formatting are not preserved.
- Punctuation is retained if it is present in the input tokens.
- Each generated n-gram is separately allocated. The returned list must be freed using `nlp_token_list_free()` to avoid memory leaks.
- The function does not perform tokenisation or text normalisation. Input tokens must already be prepared.

### Other algorithms

- Similarity measures and vectorisation methods use basic mathematical approaches. Their results may not capture deeper semantic relationships between words.
- The accuracy of these methods depends on the quality and preprocessing of the input text.

## 4. Memory and Safety Limitations

- Several functions work on fixed-size arrays. Make sure your arrays are big enough, or you may overwrite memory.
- Strings must end with a null character (`\0`). Text that is not properly terminated can cause crashes.
- You are responsible for freeing memory returned by the library. Forgetting to do so causes memory leaks. See the table in [Compilation and Usage](Compilation_and_Usage.md).
- Always check for `NULL` or `-1` return values before using a result.

## 5. Possible Future Improvements

- Support for more languages.
- Smarter sentence splitting that understands abbreviations.
- Tokenisation that keeps whole numbers together.
- A tokeniser that does not depend on a global array.
- Support for more classifiers and similarity methods.
- Improved sentiment analysis using more advanced language-processing techniques.
- More advanced keyword extraction methods that consider word importance and context.
- Better handling of multi-word expressions and semantic relationships.

