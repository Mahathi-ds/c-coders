
#include <stdio.h>
#include "project.h"

int main(void)
{
    const char *files[] = {
        "examples/tfidf_doc1.txt",
        "examples/tfidf_doc2.txt",
        "examples/tfidf_doc3.txt"
    };

    double score;
    int result;

    result = calculate_tfidf(
        "examples/tfidf_doc1.txt",
        "learning",
        files,
        3,
        &score
    );

    if(result == 1)
    {
        printf("TF-IDF Score: %lf\n", score);
    }
    else
    {
        printf("Error calculating TF-IDF\n");
    }

    return 0;
}
