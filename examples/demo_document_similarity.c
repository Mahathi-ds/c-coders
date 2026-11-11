#include <stdio.h>
#include "project.h"

#define GLOVE_FILE "data/glove.6B.100d.txt"

int main(void)
{
    char file1[500];
    char file2[500];
    double similarity;
    double distance;
    int status;

    printf("\n============================================\n");
    printf("          DOCUMENT SIMILARITY TOOL\n");
    printf("============================================\n");

    printf("\nEnter first document filename: ");
    scanf(" %499s", file1);

    printf("Enter second document filename: ");
    scanf(" %499s", file2);

    printf("\n============================================\n");
    printf("         DOCUMENT COMPARISON RESULTS\n");
    printf("============================================\n");

    printf("\nDocument 1: %s\n", file1);
    printf("Document 2: %s\n", file2);

    printf("\n1. COSINE SIMILARITY\n");
    status = calculate_cosine_similarity(file1, file2, &similarity);
    if (status)
        printf("Score: %.4f\n", similarity);
    else
        printf("Calculation failed\n");

    printf("\n2. JACCARD SIMILARITY\n");
    status = calculate_jaccard_similarity_files(file1, file2, &similarity);
    if (status)
        printf("Score: %.4f\n", similarity);
    else
        printf("Calculation failed\n");

    printf("\n3. DICE SIMILARITY\n");
    status = calculate_dice_similarity(file1, file2, &similarity);
    if (status)
        printf("Score: %.4f\n", similarity);
    else
        printf("Calculation failed\n");

    printf("\n4. OVERLAP COEFFICIENT\n");
    status = calculate_overlap_coefficient(file1, file2, &similarity);
    if (status)
        printf("Score: %.4f\n", similarity);
    else
        printf("Calculation failed\n");

    printf("\n5. EUCLIDEAN DISTANCE\n");
    status = calculate_euclidean_distance(file1, file2, &distance);
    if (status)
        printf("Distance: %.4f\n", distance);
    else
        printf("Calculation failed\n");

    printf("\n6. MANHATTAN DISTANCE\n");
    status = calculate_manhattan_distance(file1, file2, &distance);
    if (status)
        printf("Distance: %.4f\n", distance);
    else
        printf("Calculation failed\n");

    printf("\n7. SEMANTIC SIMILARITY (GloVe)\n");
    printf("Loading GloVe embeddings, please wait...\n");
    status = calculate_semantic_similarity(file1, file2, GLOVE_FILE,
                                           &similarity);
    if (status)
        printf("Score: %.4f\n", similarity);
    else
        printf("Calculation failed\n");

    printf("\n============================================\n");
    printf("             COMPARISON COMPLETED\n");
    printf("============================================\n");

    return 0;
}