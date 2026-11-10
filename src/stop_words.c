#include<stdio.h>
#include "project.h"
#include<stdlib.h>
#include<string.h>

int is_negation(char *word)
{
    char *negations[] = {"no", "not", "nor"};

    for (int i = 0; i < 3; i++)
    {
        if (strcmp(word, negations[i]) == 0)
            return 1;
    }

    return 0;
}
int stop_words(char stopwords[][100]){


    FILE *fp = fopen("data/EN-Stopwords.txt", "r");
    if(fp==NULL){
        perror("Error opening stopwords file");
        return -1;
    }
    int count=0;
    while (count<1000 && fscanf(fp, "%99s", stopwords[count]) == 1) {
        count++;
    }
    fclose(fp);
    return count;
}

int remove_stopwords(struct word *tokens,int keep_negations){
    if(tokens==NULL || rear==0){
        return -1;
    }
    char stopwords[1000][100];
    int count = stop_words(stopwords);

    if (count == -1) {
        return -1;
    }

    int i = 0;
    while (i < rear) {
        make_lower(tokens[i].data);

        int found=0;
        for (int j=0;j<count;j++){
            if(strcmp(tokens[i].data,stopwords[j])==0){
                if (keep_negations && is_negation(tokens[i].data))
                    break;
                for(int k=i;k<rear-1;k++){
                    tokens[k]=tokens[k+1];
                }
                rear--;
                found=1;
                break;
            }
            
        }
            if(found==0){
                i++;
        }
    }
    return 0;
    
}

