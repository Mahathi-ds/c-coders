# Architecture and Design

## 1. Introduction

The project follows a modular architecture in which different Natural Language Processing (NLP) functionalities are implemented as independent modules. Each module is responsible for a specific operation and exposes its functionality through functions declared in a common header file, `project.h`.

The architecture is designed to promote code reusability, maintainability, and ease of integration. Users can either use individual functions independently or combine multiple functionalities to perform complete text-processing and analysis tasks.

The project is implemented in C, with separate directories for header files, source files, examples, tests, and documentation.

## 2. Project Architecture

The project is organised into the following directories:

```text
project/
│
├── include/
│   └── project.h
│
├── src/
│   ├── text_normalisation.c
│   ├── sentence_split.c
│   ├── word_tokenize.c
│   ├── case_normalisation.c
│   ├── stop_words.c
│   ├── vectorizer.c
│   ├── naive_bayes.c
│   └── jaccard.c
    

│
├── data/
│   └── EN-Stopwords.txt
    
│
├── examples/
│   └── demo.c
│
├── tests/
│
├── documentation/
│
├── Makefile
└── README.md
```

### 2.1 Directory Description

| Directory/File | Purpose |
|---|---|
| `include/` | Contains the public header file declaring the functions and data structures provided by the library. |
| `src/` | Contains the implementation of the different NLP functionalities. |
| `data/` | Contains data files used by the library, such as the English stop-word list (`EN-Stopwords.txt`). |
| `examples/` | Contains demonstration programs showing how to use the library. |
| `tests/` | Contains test programs used to verify the correctness of individual modules and algorithms. |
| `documentation/` | Contains detailed project documentation. |
| `Makefile` | Automates compilation and linking of the project files. |
| `README.md` | Provides a brief introduction and instructions for getting started. |

## 3. Modular Design

The library is divided into the following functionalalities.

### 3.1 Text Preprocessing Module

This module provides the fundamental operations required to prepare raw text for further analysis.

It includes:

- Text normalisation.
- Sentence splitting.
- Word tokenisation.
- Case normalisation.
- Stop-word removal.
- Basic stemming.

Each operation is implemented through separate functions, allowing users to perform preprocessing operations independently or combine them into a processing pipeline.

### 3.2 Text Representation Module

This module converts textual data into structured representations that can be used by NLP algorithms.

Its functionalities include:

- Word frequency analysis.
- N-gram generation.
- TF-IDF vectorisation.

The vocabulary provides a mapping between words and their corresponding positions in the numerical representation of documents.

### 3.3 NLP Algorithms Module

This module contains algorithms that operate on processed text or its numerical representations.

The algorithms include:

- Naive Bayes text classification.
- Sentiment scoring.
- Keyword extraction.
- Document similarity.

These algorithms enable document comparison, classification, and text analysis.


## 4. Data Flow and Module Interaction

The data flows through different modules depending on the operation being performed.

1. **Input:** Text can be provided directly as strings or read from external text files.

2. **Preprocessing:** The input text is processed using the available normalisation, tokenisation, stop-word removal, and stemming functions.

3. **Text Representation:** Processed documents are converted into vocabularies, word-frequency representations, or numerical vectors.

4. **Algorithm Execution:** The resulting representations are supplied to the required NLP algorithms, such as similarity measures or Naive Bayes classification.

5. **Output:** The library returns the processed text, numerical representations, predicted classes, similarity scores, or other analysis results.

The common header file, `project.h`, acts as the interface between the library modules and the programs that use them.

## 5. Design Principles

The following principles guide the design of the library.

### 5.1 Modularity

Each functionality is implemented independently, making it easier to modify, debug, test, and extend individual modules without affecting unrelated functionalities.

### 5.2 Reusability

Functions are designed to be reusable across different applications. Users can invoke individual functions without having to execute the entire processing pipeline.

### 5.3 Encapsulation

Implementation details are maintained in the respective source files, while function declarations and required data structure definitions are exposed through the public header file.

### 5.4 Dynamic Memory Management

Dynamic memory allocation is used wherever the size of the data structures depends on the input. Appropriate memory deallocation is provided to prevent memory leaks and allow repeated use of the library.



## 6. Compilation and Integration

The source files are compiled and linked together to create an executable demonstration application or to integrate the library functionalities into another C program.

The public header file is included in programs that use the library:

```c
#include "project.h"
```

The Makefile provides a convenient way to compile the required source files and link them with the demonstration program.

Detailed compilation commands and usage instructions are provided in the Compilation and Usage section of the documentation.