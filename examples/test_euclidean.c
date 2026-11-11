
#include <stdio.h>
#include "project.h"

int main()
{
    double distance;

    if(calculate_euclidean_distance(
        "examples/doc1.txt",
        "examples/doc2.txt",
        &distance))
    {
        printf("\nEuclidean Distance = %.4f\n", distance);
    }
    else
    {
        printf("Error calculating Euclidean Distance\n");
    }

    return 0;
}
