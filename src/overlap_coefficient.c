
#include <stdio.h>
#include "project.h"


int calculate_overlap_coefficient(
    const char *file1,
    const char *file2,
    double *similarity)
{
    FreqList list1;
    FreqList list2;

    size_t i;

    int common_words = 0;
    size_t smaller_size;

    if(file1 == NULL || file2 == NULL || similarity == NULL)
    {
        return 0;
    }

    *similarity = 0.0;



    if(!sim_build_frequency_list(file1, &list1))
    {
        return 0;
    }

    if(!sim_build_frequency_list(file2, &list2))
    {
        sim_free_frequency_list(&list1);
        return 0;
    }



    for(i = 0; i < list1.count; i++)
    {
        if(sim_find_word(&list2, list1.frequencies[i].word) != -1)
        {
            common_words++;
        }
    }



    if(list1.count < list2.count)
    {
        smaller_size = list1.count;
    }
    else
    {
        smaller_size = list2.count;
    }



    if(smaller_size != 0)
    {
        *similarity = (double)common_words / smaller_size;
    }



    sim_free_frequency_list(&list1);
    sim_free_frequency_list(&list2);

    return 1;
}
