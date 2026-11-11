# Limitations

This library is built to teach and demonstrate basic NLP in C. It is simple on purpose, so it has some limits. Knowing them will help you avoid surprises.

## 1. General Limitations

| Limitation | What it means for you |
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

### N-gram generation

- Returns `NULL` if there are fewer tokens than `n`.
- Large values of `n` on short texts produce few or no results.

### Stemming

- Follows the Porter algorithm, which uses fixed rules. Some stems are not real words (for example, `studies` becomes `studi`).
- Some different words may end up with the same stem, and some related words may not.
- Words with two or fewer characters are not changed.

## 3. Algorithm Limitations

### Naive Bayes

- Treats every word as independent of the others, so word order and context are ignored.
- Needs labelled training data. Very small training sets give unreliable predictions.
- Can only predict labels it saw during training.
- Words that were not in the training data are not helpful for prediction.
- You must free the model and the predictions yourself. See [Algorithms](Algorithms.md).

### Other algorithms

- Similarity, sentiment scoring and keyword extraction are basic methods and may not match the accuracy of advanced tools.

> **To be added:** any further limits found while testing vectorisation, TF-IDF, cosine similarity, Jaccard similarity, sentiment scoring and keyword extraction.

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