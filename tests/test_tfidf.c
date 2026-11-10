
#include <stdio.h>
#include <math.h>
#include "project.h"

int create_test_file(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        return 0;
    }

    if (fputs(content, file) == EOF)
    {
        fclose(file);
        return 0;
    }

    if (fclose(file) != 0)
    {
        return 0;
    }

    return 1;
}

void check_test(const char *name, int passed)
{
    if (passed)
    {
        printf("PASS: %s\n", name);
    }
    else
    {
        printf("FAIL: %s\n", name);
    }
}

int main(void)
{
    const char *documents[] = {
        "examples/tfidf_doc1.txt",
        "examples/tfidf_doc2.txt",
        "examples/tfidf_doc3.txt"
    };

    int result;
    double score;
    double expected_score;

    printf("===== TF-IDF TESTS =====\n\n");

    /* Create three test documents */

    if (!create_test_file(documents[0],
                          "Apple apple banana."))
    {
        printf("FAIL: Could not create document 1\n");
        return 1;
    }

    if (!create_test_file(documents[1],
                          "banana orange."))
    {
        printf("FAIL: Could not create document 2\n");
        return 1;
    }

    if (!create_test_file(documents[2],
                          "APPLE! orange banana"))
    {
        printf("FAIL: Could not create document 3\n");
        return 1;
    }

    if (!create_test_file("examples/tfidf_empty.txt", ""))
    {
        printf("FAIL: Could not create empty document\n");
        return 1;
    }

    check_test("Test documents created", 1);

    /* Test 1: Normal calculation */

    expected_score = (2.0 / 3.0) * log(3.0 / 2.0);

    result = calculate_tfidf(
        documents[0], "apple", documents, 3, &score);

    check_test("Normal TF-IDF calculation",
               result == 1 &&
               fabs(score - expected_score) < 0.000001);

    if (result == 1)
    {
        printf("Apple TF-IDF = %.6f\n", score);
    }

    /* Test 2: Case-insensitive matching */

    result = calculate_tfidf(
        documents[0], "APPLE", documents, 3, &score);

    check_test("Case-insensitive matching",
               result == 1 &&
               fabs(score - expected_score) < 0.000001);

    /* Test 3: Word appears in every document */

    result = calculate_tfidf(
        documents[0], "banana", documents, 3, &score);

    check_test("Word in every document",
               result == 1 && score == 0.0);

    /* Test 4: Word absent from all documents */

    result = calculate_tfidf(
        documents[0], "grape", documents, 3, &score);

    check_test("Word absent from corpus",
               result == 1 && score == 0.0);

    /* Test 5: Word not in target document,
       but present in the corpus */

    result = calculate_tfidf(
        documents[1], "apple", documents, 3, &score);

    check_test("Word absent from target document",
               result == 1 && score == 0.0);

    /* Test 6: Empty target document */

    result = calculate_tfidf(
        "examples/tfidf_empty.txt", "apple",
        documents, 3, &score);

    check_test("Empty target document",
               result == 1 && score == 0.0);

    /* Test 7: Missing target file */

    result = calculate_tfidf(
        "examples/tfidf_missing.txt", "apple",
        documents, 3, &score);

    check_test("Missing target file",
               result == 0);

    /* Test 8: Zero documents */

    result = calculate_tfidf(
        documents[0], "apple", documents, 0, &score);

    check_test("Zero document count",
               result == 0);

    /* Test 9: Empty target word */

    result = calculate_tfidf(
        documents[0], "", documents, 3, &score);

    check_test("Empty target word",
               result == 0);

    /* Test 10: NULL filename */

    result = calculate_tfidf(
        NULL, "apple", documents, 3, &score);

    check_test("NULL filename",
               result == 0);

    /* Test 11: NULL target word */

    result = calculate_tfidf(
        documents[0], NULL, documents, 3, &score);

    check_test("NULL target word",
               result == 0);

    /* Test 12: NULL document collection */

    result = calculate_tfidf(
        documents[0], "apple", NULL, 3, &score);

    check_test("NULL document collection",
               result == 0);

    /* Test 13: NULL score pointer */

    result = calculate_tfidf(
        documents[0], "apple", documents, 3, NULL);

    check_test("NULL score pointer",
               result == 0);

    printf("\n===== TESTING COMPLETE =====\n");

    return 0;
}
