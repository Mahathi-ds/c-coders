
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project.h"

#define NUM_TRAIN 32
#define NUM_TEST 8

// Read the complete contents of a text file
char *read_file(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL) {
        perror(filename);
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return NULL;
    }

    long size = ftell(fp);

    if (size < 0) {
        fclose(fp);
        return NULL;
    }

    rewind(fp);

    char *text = malloc((size_t)size + 1);

    if (text == NULL) {
        fclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(text, 1, (size_t)size, fp);

    if (bytes_read != (size_t)size || ferror(fp)) {
        free(text);
        fclose(fp);
        return NULL;
    }

    text[bytes_read] = '\0';

    fclose(fp);

    return text;
}


// Free an array of dynamically allocated strings
void free_documents(char **docs, int count)
{
    if (docs == NULL)
        return;

    for (int i = 0; i < count; i++)
        free(docs[i]);

    free(docs);
}


int main()
{
    // =========================================
    // 1. TRAINING FILE PATHS
    // =========================================

    const char *train_paths[NUM_TRAIN] = {

        // Sports
        "data/corpus/train/sports/doc1.txt",
        "data/corpus/train/sports/doc2.txt",
        "data/corpus/train/sports/doc3.txt",
        "data/corpus/train/sports/doc4.txt",
        "data/corpus/train/sports/doc5.txt",
        "data/corpus/train/sports/doc6.txt",
        "data/corpus/train/sports/doc7.txt",
        "data/corpus/train/sports/doc8.txt",



        // Technology
        "data/corpus/train/technology/doc1.txt",
        "data/corpus/train/technology/doc2.txt",
        "data/corpus/train/technology/doc3.txt",
        "data/corpus/train/technology/doc4.txt",
        "data/corpus/train/technology/doc5.txt",
        "data/corpus/train/technology/doc6.txt",
         "data/corpus/train/technology/doc7.txt",
        "data/corpus/train/technology/doc8.txt",

        // Health
        "data/corpus/train/health/doc1.txt",
        "data/corpus/train/health/doc2.txt",
        "data/corpus/train/health/doc3.txt",
        "data/corpus/train/health/doc4.txt",
        "data/corpus/train/health/doc5.txt",
        "data/corpus/train/health/doc6.txt",
         "data/corpus/train/health/doc7.txt",
        "data/corpus/train/health/doc8.txt",
        

        // Politics
        "data/corpus/train/politics/doc1.txt",
        "data/corpus/train/politics/doc2.txt",
        "data/corpus/train/politics/doc3.txt",
        "data/corpus/train/politics/doc4.txt",
        "data/corpus/train/politics/doc5.txt",
        "data/corpus/train/politics/doc6.txt",
         "data/corpus/train/politics/doc7.txt",
        "data/corpus/train/politics/doc8.txt"
        
    };


    // Corresponding labels
    char *labels[NUM_TRAIN] = {

        "Sports", "Sports", "Sports",
        "Sports", "Sports", "Sports","SSports","Sports",

        "Technology", "Technology", "Technology",
        "Technology", "Technology", "Technology","Technology","Technology",

        "Health", "Health", "Health",
        "Health", "Health", "Health","Health","Health",

        "Politics", "Politics", "Politics",
        "Politics", "Politics", "Politics","Politics","Politics"
    };


    // =========================================
    // 2. LOAD TRAINING DOCUMENTS
    // =========================================

    char **docs = calloc(NUM_TRAIN, sizeof(char *));

    if (docs == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    int loaded = 0;

    for (int i = 0; i < NUM_TRAIN; i++) {

        docs[i] = read_file(train_paths[i]);

        if (docs[i] == NULL) {
            printf("Could not load training document: %s\n",
                   train_paths[i]);

            free_documents(docs, loaded);
            return 1;
        }

        loaded++;
    }

    printf("Loaded %d training documents.\n\n", loaded);


    // =========================================
    // 3. TRAIN NAIVE BAYES MODEL
    // =========================================

    NaiveBayesModel model =
        Naivebayes_fit(docs, labels, NUM_TRAIN);

    printf("Training completed.\n\n");


    // =========================================
    // 4. TEST FILE PATHS
    // =========================================

    const char *test_paths[NUM_TEST] = {

        "data/corpus/test/test1.txt",
        "data/corpus/test/test2.txt",
        "data/corpus/test/test3.txt",
        "data/corpus/test/test4.txt",
        "data/corpus/test/test5.txt",
        "data/corpus/test/test6.txt",
        "data/corpus/test/test7.txt",
        "data/corpus/test/test8.txt"
    };


    // Expected labels are used ONLY for evaluation.
    // They are NOT passed to Naivebayes_predict().

    char *expected_labels[NUM_TEST] = {
        "Sports",
        "Sports",
        "Technology",
        "Technology",
        "Health",
        "Health",
        "Politics",
        "Politics"
    };


    // =========================================
    // 5. LOAD TEST DOCUMENTS
    // =========================================

    char **test_docs = calloc(NUM_TEST, sizeof(char *));

    if (test_docs == NULL) {
        printf("Memory allocation failed.\n");

        NaiveBayes_free(&model);
        free_documents(docs, NUM_TRAIN);
        return 1;
    }

    loaded = 0;

    for (int i = 0; i < NUM_TEST; i++) {

        test_docs[i] = read_file(test_paths[i]);

        if (test_docs[i] == NULL) {
            printf("Could not load test document: %s\n",
                   test_paths[i]);

            free_documents(test_docs, loaded);
            NaiveBayes_free(&model);
            free_documents(docs, NUM_TRAIN);
            return 1;
        }

        loaded++;
    }

    printf("Loaded %d test documents.\n\n", loaded);


    // =========================================
    // 6. PREDICT
    // =========================================

    char **predictions =
        Naivebayes_predict(model, test_docs, NUM_TEST);


    if (predictions == NULL) {
        printf("Prediction failed.\n");

        free_documents(test_docs, NUM_TEST);
        NaiveBayes_free(&model);
        free_documents(docs, NUM_TRAIN);
        return 1;
    }


    // =========================================
    // 7. DISPLAY RESULTS AND CALCULATE ACCURACY
    // =========================================

    int correct = 0;

    for (int i = 0; i < NUM_TEST; i++) {

        printf("Test document %d\n", i + 1);

        printf("Expected class : %s\n",
               expected_labels[i]);

        printf("Predicted class: %s\n",
               predictions[i] ? predictions[i] : "NULL");

        if (predictions[i] != NULL &&
            strcmp(predictions[i], expected_labels[i]) == 0) {

            printf("Result         : PASS\n");
            correct++;
        }
        else {
            printf("Result         : FAIL\n");
        }

        printf("\n");
    }


    printf("=================================\n");
    printf("Correct predictions: %d/%d\n", correct, NUM_TEST);

    printf("Accuracy: %.2f%%\n",
           (correct * 100.0) / NUM_TEST);


    // =========================================
    // 8. MEMORY CLEANUP
    // =========================================

    // Free each predicted label and prediction array
    for (int i = 0; i < NUM_TEST; i++)
        free(predictions[i]);

    free(predictions);

    // Free test documents
    free_documents(test_docs, NUM_TEST);

    // Free trained model
    NaiveBayes_free(&model);

    // Free training documents
    free_documents(docs, NUM_TRAIN);

    printf("\nMemory cleanup completed.\n");

    return 0;
}
