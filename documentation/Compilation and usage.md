# Compilation and Usage

This guide explains how to compile the NLP library and use its functions in your own C programs.

## 1. Requirements

- GCC compiler.
- The project directory containing `include/`, `src/`, and `data/`.
- `make` (optional, if using the Makefile).

To check whether GCC is installed:

```bash
gcc --version
```

## 2. Compiling Your Program

Open a terminal in the project root directory.

Compile your program together with the library source files:

```bash
gcc -Iinclude src/*.c my_program.c -o my_program -lm
```

- `-Iinclude` specifies the location of `project.h`.
- `src/*.c` includes the library implementation files.
- `my_program.c` is your program.
- `-lm` links the mathematics library.

Run the compiled program:

```bash
./my_program
```

On Windows, run it using:

```powershell
.\my_program.exe
```

**Important:** Run your program from the project root directory because stop-word removal reads `data/EN-Stopwords.txt` using a relative path.

If you use the Makefile, you can build the project with:

```bash
make
```

## 3. Using the Library

Include the library header in your C program:

```c
#include "project.h"
```

You can then call the required library functions. Compile your program together with the source files as shown in Section 2.

**Note:** Do not compile another example or demo file containing `main()` along with your own program, since a C executable can have only one `main()` function.

## 4. Memory Management

Some library functions allocate memory dynamically. The caller is responsible for freeing the returned memory where applicable.

| Function                                                                  | Memory management                                                                                                                        |
| ------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------- |
| `text_normalization()`, `make_lower()`, `make_upper()`, `nlp_stem_word()` | Modify the input string in place. No separate result allocation needs to be freed.                                                       |
| `sentence_split()`                                                        | Uses the output array supplied by the caller.                                                                                            |
| `word_tokenize()`                                                         | Returns a pointer to a global token array. Do not free it. A subsequent call overwrites its contents.                                    |
| `nlp_word_frequency()`                                                    | Free the returned `FreqList` and its `frequencies` array. The words are borrowed from the input tokens and must not be freed separately. |
| `nlp_generate_ngrams()`                                                   | Free the dynamically allocated n-gram strings and the returned `TokenList` using the appropriate cleanup procedure.                      |
| `Naivebayes_fit()`                                                        | Call `NaiveBayes_free(&model)` when the trained model is no longer needed.                                                               |
| `Naivebayes_predict()`                                                    | Free each predicted label and then the predictions array.                                                                                |

Always follow the ownership rules of each function. Do not free memory owned by the library or free the same allocation more than once.

For structure definitions and ownership details, refer to `Data_Structures.md`.

