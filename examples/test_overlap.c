
#include <stdio.h>
#include "project.h"

int main()
{
    double similarity;

    if(calculate_overlap_coefficient(
        "examples/doc1.txt",
        "examples/doc2.txt",
        &similarity))
    {
        printf("\nOverlap Coefficient = %.4f\n", similarity);
    }
    else
    {
        printf("Error calculating Overlap Coefficient\n");
    }

    return 0;
}
