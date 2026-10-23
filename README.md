# c-coders

# 1. Text Normalization
gcc -Iinclude tests/test_text_normalisation.c src/text_normalisation.c -o test_text_normalisation.exe

# 2. Case Normalization
gcc -Iinclude tests/test_case_normalisation.c src/case_normalisation.c -o test_case_normalisation.exe

# 3. Sentence Splitting
gcc -Iinclude tests/test_sentence_splitting.c src/sentence_split.c -o test_sentence_splitting.exe

# 4. Word Tokenization
gcc -Iinclude tests/test_tokenize.c src/tokenize.c -o test_tokenize.exe

# 5. Stop-Word Removal
gcc -Iinclude tests/test_stop_words.c src/stop_words.c -o test_stop_words.exe

# 6. Word Frequency Analysis
gcc -Iinclude tests/test_frequency.c src/frequency.c -o test_frequency.exe

# 7. N-gram Generation
gcc -Iinclude tests/ng.c src/ngrams.c -o test_ngrams.exe

# 8. Stemming
gcc -Iinclude tests/test_stemmer.c src/stem.c src/stemmer.c -o test_stemmer.exe


# 10. Complete NLP Pipeline
gcc -Iinclude examples/demo_pipeline.c src/text_normalisation.c src/sentence_split.c src/tokenize.c src/case_normalisation.c src/stop_words.c src/ngrams.c src/stemmer.c src/frequency.c -o demo_pipeline.exe