#include <stdio.h>
#include "project.h"

int main(void)
{
    const char *file1 = "examples/cosine_doc1.txt";
    const char *file2 = "examples/cosine_doc2.txt";

    double similarity;

    int result;

    result = calculate_cosine_similarity(
        file1,
        file2,
        &similarity
    );

    if(result == 1)
    {
        printf("Cosine Similarity: %lf\n", similarity);
    }
    else
    {
        printf("Error calculating cosine similarity\n");
    }

    return 0;
}