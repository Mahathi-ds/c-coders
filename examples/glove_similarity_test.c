
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project.h"

int main()
{
    GloveModel model;

    float similarity;
    float document_vector[GLOVE_DIM];

    char word1[256];
    char word2[256];

    char document1[1000];
    char document2[1000];

    int found;
    int i;

    GloveWord *result;

    /* Initialize model */

    model.words = NULL;
    model.hash_table = NULL;
    model.word_count = 0;

    printf("====================================\n");
    printf("       GLOVE SIMILARITY TEST\n");
    printf("====================================\n");

    /* Load GloVe model */

    printf("\nLoading GloVe model...\n");

    if (glove_load("data/glove.6B.50d.txt", &model) == 0)
    {
        printf("\nGloVe loading failed.\n");
        return 1;
    }

    printf("\nGloVe model loaded successfully!\n");

    /* TEST 1: WORD LOOKUP */

    printf("\n====================================\n");
    printf("TEST 1: WORD LOOKUP\n");
    printf("====================================\n");

    while (1)
    {
        printf("\nEnter a word (type exit to stop): ");

        if (fgets(word1, sizeof(word1), stdin) == NULL)
        {
            break;
        }

        word1[strcspn(word1, "\n")] = '\0';

        if (strcmp(word1, "exit") == 0)
        {
            break;
        }

        result = glove_lookup(&model, word1);

        if (result != NULL)
        {
            printf("Word found: %s\n", result->word);

            printf("First 10 embedding values:\n");

            for (i = 0; i < 10 && i < GLOVE_DIM; i++)
            {
                printf("%.4f ", result->vector[i]);
            }

            printf("\n");
        }
        else
        {
            printf("Word not found.\n");
        }
    }

    /* TEST 2: WORD SEMANTIC SIMILARITY */

    printf("\n====================================\n");
    printf("TEST 2: WORD SEMANTIC SIMILARITY\n");
    printf("====================================\n");

    while (1)
    {
        printf("\nEnter first word (type exit to stop): ");

        if (fgets(word1, sizeof(word1), stdin) == NULL)
        {
            break;
        }

        word1[strcspn(word1, "\n")] = '\0';

        if (strcmp(word1, "exit") == 0)
        {
            break;
        }

        printf("Enter second word: ");

        if (fgets(word2, sizeof(word2), stdin) == NULL)
        {
            break;
        }

        word2[strcspn(word2, "\n")] = '\0';

        similarity = glove_word_similarity(
            &model, word1, word2
        );

        if (similarity >= -1.0f)
        {
            printf("Word similarity: %.4f\n", similarity);
        }
        else
        {
            printf("One or both words were not found.\n");
        }
    }

    /* TEST 3: DOCUMENT VECTOR */

    printf("\n====================================\n");
    printf("TEST 3: DOCUMENT VECTOR\n");
    printf("====================================\n");

    while (1)
    {
        printf("\nEnter a document (type exit to stop): ");

        if (fgets(document1, sizeof(document1), stdin) == NULL)
        {
            break;
        }

        document1[strcspn(document1, "\n")] = '\0';

        if (strcmp(document1, "exit") == 0)
        {
            break;
        }

        found = glove_document_vector(
            &model, document1, document_vector
        );

        printf("Words found in GloVe: %d\n", found);

        if (found > 0)
        {
            printf("Document vector:\n");

            for (i = 0; i < GLOVE_DIM; i++)
            {
                printf("%.4f ", document_vector[i]);
            }

            printf("\n");
        }
        else
        {
            printf("No words found in GloVe.\n");
        }
    }

    /* TEST 4: DOCUMENT SEMANTIC SIMILARITY */

    printf("\n====================================\n");
    printf("TEST 4: DOCUMENT SEMANTIC SIMILARITY\n");
    printf("====================================\n");

    while (1)
    {
        printf("\nEnter first document (type exit to stop): ");

        if (fgets(document1, sizeof(document1), stdin) == NULL)
        {
            break;
        }

        document1[strcspn(document1, "\n")] = '\0';

        if (strcmp(document1, "exit") == 0)
        {
            break;
        }

        printf("Enter second document: ");

        if (fgets(document2, sizeof(document2), stdin) == NULL)
        {
            break;
        }

        document2[strcspn(document2, "\n")] = '\0';

        similarity = glove_document_similarity(
            &model, document1, document2
        );

        if (similarity >= 0.0f)
        {
            printf("\nDocument similarity: %.4f\n", similarity);
        }
        else
        {
            printf("\nCould not calculate document similarity.\n");
        }
    }

    /* RELEASE MEMORY */

    printf("\n====================================\n");
    printf("RELEASING MEMORY\n");
    printf("====================================\n");

    glove_free(&model);

    printf("GloVe memory released successfully.\n");

    printf("\nTesting completed!\n");

    return 0;
}
