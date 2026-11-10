
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project.h"

#define LARGE_DATASET_SIZE 100

/* Helper function to release predictions */
void free_predictions(char **predictions, int count)
{
    if (predictions == NULL)
        return;

    for (int i = 0; i < count; i++)
        free(predictions[i]);

    free(predictions);
}

/* Helper function to display predictions */
void display_predictions(char **predictions, int count)
{
    if (predictions == NULL)
    {
        printf("Prediction failed.\n");
        return;
    }

    for (int i = 0; i < count; i++)
        printf("Document %d: %s\n", i + 1, predictions[i]);
}


int main(void)
{
    NaiveBayesModel model = {0};
    char **predictions = NULL;

    /* ==========================================
       TEST 1: NORMAL CASE
       ========================================== */

    printf("\n====================================\n");
    printf("TEST 1: NORMAL CASE\n");
    printf("====================================\n");

    char *normal_docs[] = {
        "excellent acting wonderful story",
        "great movie loved it",
        "amazing film enjoyable experience",
        "boring plot terrible acting",
        "bad movie hated it",
        "awful film dull experience"
    };

    char *normal_labels[] = {
        "positive",
        "positive",
        "positive",
        "negative",
        "negative",
        "negative"
    };

    char *normal_test[] = {
        "wonderful movie excellent acting",
        "terrible boring film",
        "great enjoyable experience",
        "bad dull movie"
    };

    model = Naivebayes_fit(normal_docs, normal_labels, 6);

    if (model.vocab.words == NULL)
    {
        printf("Training failed.\n");
    }
    else
    {
        printf("Training successful.\n");

        predictions = Naivebayes_predict(model, normal_test, 4);

        display_predictions(predictions, 4);

        free_predictions(predictions, 4);
        NaiveBayes_free(&model);
    }


    /* ==========================================
       TEST 2: BOUNDARY CASES
       One document and one class
       ========================================== */

    printf("\n====================================\n");
    printf("TEST 2: BOUNDARY CASES\n");
    printf("====================================\n");

    char *boundary_docs[] = {
        "excellent wonderful amazing"
    };

    char *boundary_labels[] = {
        "positive"
    };

    char *boundary_test[] = {
        "excellent",
        "unknown vocabulary words"
    };

    model = Naivebayes_fit(boundary_docs, boundary_labels, 1);

    if (model.vocab.words == NULL)
    {
        printf("Boundary training failed.\n");
    }
    else
    {
        printf("Single-document training successful.\n");

        predictions = Naivebayes_predict(model, boundary_test, 2);

        display_predictions(predictions, 2);

        free_predictions(predictions, 2);
        NaiveBayes_free(&model);
    }


    /* ==========================================
       TEST 3: INVALID INPUT
       ========================================== */

    printf("\n====================================\n");
    printf("TEST 3: INVALID INPUT\n");
    printf("====================================\n");

    model = Naivebayes_fit(NULL, normal_labels, 6);

    if (model.vocab.words == NULL)
        printf("NULL documents handled correctly.\n");

    NaiveBayes_free(&model);

    model = Naivebayes_fit(normal_docs, NULL, 6);

    if (model.vocab.words == NULL)
        printf("NULL labels handled correctly.\n");

    NaiveBayes_free(&model);

    model = Naivebayes_fit(normal_docs, normal_labels, 0);

    if (model.vocab.words == NULL)
        printf("Zero document count handled correctly.\n");

    NaiveBayes_free(&model);


    /* ==========================================
       TEST 4: EMPTY INPUT
       Empty document and empty prediction
       ========================================== */

    printf("\n====================================\n");
    printf("TEST 4: EMPTY INPUT\n");
    printf("====================================\n");

    char *empty_docs[] = {
        "",
        "excellent movie",
        "terrible movie"
    };

    char *empty_labels[] = {
        "neutral",
        "positive",
        "negative"
    };

    char *empty_test[] = {
        "",
        "excellent",
        "terrible"
    };

    model = Naivebayes_fit(empty_docs, empty_labels, 3);

    if (model.vocab.words == NULL)
    {
        printf("Training with an empty document failed.\n");
    }
    else
    {
        printf("Training with an empty document successful.\n");

        predictions = Naivebayes_predict(model, empty_test, 3);

        display_predictions(predictions, 3);

        free_predictions(predictions, 3);
        NaiveBayes_free(&model);
    }


    /* ==========================================
       TEST 5: LARGE INPUT
       ========================================== */

    printf("\n====================================\n");
    printf("TEST 5: LARGE INPUT\n");
    printf("====================================\n");

    char large_docs[LARGE_DATASET_SIZE][256];
    char large_labels[LARGE_DATASET_SIZE][20];

    char *large_doc_ptrs[LARGE_DATASET_SIZE];
    char *large_label_ptrs[LARGE_DATASET_SIZE];

    for (int i = 0; i < LARGE_DATASET_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            snprintf(large_docs[i], sizeof(large_docs[i]),
                     "excellent wonderful amazing positive movie %d", i);

            strcpy(large_labels[i], "positive");
        }
        else
        {
            snprintf(large_docs[i], sizeof(large_docs[i]),
                     "terrible awful boring negative movie %d", i);

            strcpy(large_labels[i], "negative");
        }

        large_doc_ptrs[i] = large_docs[i];
        large_label_ptrs[i] = large_labels[i];
    }

    model = Naivebayes_fit(
        large_doc_ptrs,
        large_label_ptrs,
        LARGE_DATASET_SIZE
    );

    if (model.vocab.words == NULL)
    {
        printf("Large dataset training failed.\n");
    }
    else
    {
        printf("Successfully trained on %d documents.\n",
               LARGE_DATASET_SIZE);

        predictions = Naivebayes_predict(
            model,
            normal_test,
            4
        );

        display_predictions(predictions, 4);

        free_predictions(predictions, 4);
        NaiveBayes_free(&model);
    }


    /* ==========================================
       TEST 6: FAILURE CONDITIONS
       ========================================== */

    printf("\n====================================\n");
    printf("TEST 6: FAILURE CONDITIONS\n");
    printf("====================================\n");

    NaiveBayesModel empty_model = {0};

    predictions = Naivebayes_predict(
        empty_model,
        normal_test,
        4
    );

    if (predictions == NULL)
        printf("Prediction with an untrained model handled correctly.\n");

    predictions = Naivebayes_predict(
        empty_model,
        NULL,
        4
    );

    if (predictions == NULL)
        printf("NULL test documents handled correctly.\n");

    predictions = Naivebayes_predict(
        empty_model,
        normal_test,
        0
    );

    if (predictions == NULL)
        printf("Zero prediction count handled correctly.\n");


    /* ==========================================
       TEST 7: MEMORY MANAGEMENT
       Repeated training and prediction
       ========================================== */

    printf("\n====================================\n");
    printf("TEST 7: MEMORY MANAGEMENT\n");
    printf("====================================\n");

    int repetitions = 20;

    for (int i = 0; i < repetitions; i++)
    {
        model = Naivebayes_fit(
            normal_docs,
            normal_labels,
            6
        );

        if (model.vocab.words == NULL)
        {
            printf("Training failed at iteration %d.\n", i + 1);
            continue;
        }

        predictions = Naivebayes_predict(
            model,
            normal_test,
            4
        );

        free_predictions(predictions, 4);

        NaiveBayes_free(&model);
    }

    printf("Completed %d training-prediction-cleanup cycles.\n",
           repetitions);

    printf("\n====================================\n");
    printf("ALL TESTS COMPLETED\n");
    printf("====================================\n");

    return 0;
}
