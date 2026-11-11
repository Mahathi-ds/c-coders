
#include <stdio.h>
#include "project.h"

int main()
{
    double distance;

    if(calculate_manhattan_distance(
        "examples/doc1.txt",
        "examples/doc2.txt",
        &distance))
    {
        printf("\nManhattan Distance = %.4f\n", distance);
    }
    else
    {
        printf("Error calculating Manhattan Distance\n");
    }

    return 0;
}
