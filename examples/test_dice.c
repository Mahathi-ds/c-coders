
#include <stdio.h>
#include "project.h"

int main()
{
    double similarity;

    if(calculate_dice_similarity(
        "examples/doc1.txt",
        "examples/doc2.txt",
        &similarity))
    {
        printf("\nDice Similarity = %.4f\n", similarity);
    }
    else
    {
        printf("Error calculating Dice Similarity\n");
    }

    return 0;
}
