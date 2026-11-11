
#include <stdio.h>
#include <math.h>
#include "project.h"


int calculate_manhattan_distance(
    const char *file1,
    const char *file2,
    double *distance)
{
    FreqList list1;
    FreqList list2;

    const char *filenames[2];

    size_t i;

    double weight1;
    double weight2;

    double sum = 0.0;

    if(file1 == NULL || file2 == NULL || distance == NULL)
    {
        return 0;
    }

    *distance = 0.0;

    filenames[0] = file1;
    filenames[1] = file2;



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
        if(!calculate_tfidf(
            file1,
            list1.frequencies[i].word,
            filenames,
            2,
            &weight1))
        {
            sim_free_frequency_list(&list1);
            sim_free_frequency_list(&list2);
            return 0;
        }

        if(!calculate_tfidf(
            file2,
            list1.frequencies[i].word,
            filenames,
            2,
            &weight2))
        {
            sim_free_frequency_list(&list1);
            sim_free_frequency_list(&list2);
            return 0;
        }

        sum += fabs(weight1 - weight2);
    }



    for(i = 0; i < list2.count; i++)
    {
        if(sim_find_word(&list1, list2.frequencies[i].word) == -1)
        {
            if(!calculate_tfidf(
                file1,
                list2.frequencies[i].word,
                filenames,
                2,
                &weight1))
            {
                sim_free_frequency_list(&list1);
                sim_free_frequency_list(&list2);
                return 0;
            }

            if(!calculate_tfidf(
                file2,
                list2.frequencies[i].word,
                filenames,
                2,
                &weight2))
            {
                sim_free_frequency_list(&list1);
                sim_free_frequency_list(&list2);
                return 0;
            }

            sum += fabs(weight1 - weight2);
        }
    }



    *distance = sum;


    sim_free_frequency_list(&list1);
    sim_free_frequency_list(&list2);

    return 1;
}
