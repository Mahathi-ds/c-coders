Compilation and Usage

This guide explains how to compile the NLP library and use its functions in your own C programs.

1. Requirements
GCC compiler.
The project directory containing include/, src/, and data/.
make (optional, if using the Makefile).

To check whether GCC is installed:

bash
gcc --version

2. Compiling Your Program

Open a terminal in the project root directory.

Compile your program together with the library source files:

bash
gcc -Iinclude src/*.c my_program.c -o my_program -lm

-Iinclude specifies the location of project.h.
src/*.c includes the library implementation files.
my_program.c is your program.
-lm links the mathematics library.

Run the compiled program:

bash
./my_program


On Windows, run it using:

powershell
.\my_program.exe


Important: Run your program from the project root directory because stop-word removal and sentiment analysis read dictionary files using relative paths:

data/EN-Stopwords.txt
data/positive_words.txt
data/negative_words.txt

If you use the Makefile, you can build the project with:

bash
make

3. Using the Library

Include the library header in your C program:

c
#include "project.h"


You can then call the required library functions. Compile your program together with the source files as shown in Section 2.

The library provides several Natural Language Processing functionalities, including:

Text normalization and tokenization.
Sentence splitting and stop-word removal.
Word frequency analysis.
N-gram generation.
Porter Stemming Algorithm.
Keyword extraction.
Sentiment analysis using positive and negative dictionaries.
Multinomial Naive Bayes classification with Laplace smoothing.
Vectorization and TF-IDF.
Cosine similarity and Jaccard similarity.
Document similarity analysis.

Note: Do not compile another example or demo file containing main() along with your own program, since a C executable can have only one main() function.

4. Memory Management

Some library functions allocate memory dynamically. The caller is responsible for freeing the returned memory where applicable.

Function	Memory management
text_normalization(), make_lower(), make_upper(), nlp_stem_word()	Modify the input string in place. No separate result allocation needs to be freed.
sentence_split()	Uses the output array supplied by the caller.
word_tokenize()	Returns a pointer to a global token array. Do not free it. A subsequent call overwrites its contents.
nlp_word_frequency()	Free the returned FreqList and its frequencies array. The words are borrowed from the input tokens and must not be freed separately.
nlp_generate_ngrams()	Free the dynamically allocated N-gram strings, token array and returned TokenList using nlp_token_list_free().
nlp_extract_keywords()	Free the returned keyword strings and keyword array using nlp_keyword_list_free().
sentiment_set_word_lists()	Copies the supplied positive and negative dictionary words. The internal dictionary memory is automatically released at program exit.
sentiment_analyze()	Does not allocate or free the supplied TokenList or SentimentResult.
Naivebayes_fit()	Call NaiveBayes_free(&model) when the trained model is no longer needed.
Naivebayes_predict()	Free each predicted label and then the predictions array.
NaiveBayes_free()	Releases the memory owned by the trained model and resets its fields.
nlp_keyword_list_free()	Releases individual keyword strings and the keyword array.
nlp_token_list_free()	Releases dynamically allocated token strings, the token array and the TokenList structure.
Important Memory Management Notes

1. Input ownership

The caller owns the original document strings, token arrays and input structures unless a function explicitly documents otherwise.

2. Dynamically allocated results

Functions such as nlp_word_frequency(), nlp_generate_ngrams() and nlp_extract_keywords() return dynamically allocated results. Always use the appropriate cleanup procedure.

3. Borrowed pointers

The WordFreq.word fields refer to the original input token strings. Do not free these strings separately when releasing a frequency list.

4. Sentiment dictionaries

The sentiment analysis implementation maintains internal positive and negative dictionaries. Custom dictionaries are copied into internal memory, and cleanup is registered using atexit().

5. Naive Bayes model

The trained model owns its vocabulary, class labels and learned information. Always use:

c
NaiveBayes_free(&model);


Do not use:

c
free(model);


6. Prediction results

Naive Bayes prediction returns an array of separately allocated strings. Free each prediction before freeing the array:

c
for (int i = 0; i < num_test; i++) {
    free(predictions[i]);
}

free(predictions);


7. In-place stemming

nlp_stem_word() modifies the supplied writable character array directly. The caller retains ownership of the original buffer.

Always follow the ownership rules of each function. Do not free memory owned by the library or free the same allocation more than once.

For structure definitions and ownership details, refer to Data_Structures.md.

For function prototypes and detailed API information, refer to API_Reference.md.
