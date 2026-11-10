#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "project.h"


int find_word_jacc(char *arr[],int size,char *word){
    for(int i=0;i<size;i++){
        if(strcmp(arr[i],word)==0){
            return i;
        }
    }
    return -1;
}

VectorizerResult binary_vectorizer(char **docs,int num_docs){
    Vocab vocab_set;
    vocab_set.size = 0;
    vocab_set.capacity = 10;
    vocab_set.words = malloc(vocab_set.capacity * sizeof(char *));

    if (vocab_set.words == NULL)
        {return (VectorizerResult){NULL, 0, {NULL, 0, 0}};}
    for (int i = 0; i < num_docs; i++)
    {
        struct word *tokens = word_tokenize(docs[i]);

        for (int j = 0; j < rear; j++)
        {
            if (find_word_jacc(vocab_set.words, vocab_set.size,tokens[j].data) != -1)
                continue;

            if (vocab_set.size == vocab_set.capacity)
            {
                vocab_set.capacity *= 2;

                char **temp = realloc(
                    vocab_set.words,
                    vocab_set.capacity * sizeof(char *)
                );

                if (temp == NULL)
                    {return (VectorizerResult){NULL, 0, {NULL, 0, 0}};}

                vocab_set.words = temp;
            }

            vocab_set.words[vocab_set.size] =malloc(strlen(tokens[j].data) + 1);

            if (vocab_set.words[vocab_set.size] == NULL)
                return (VectorizerResult){NULL, 0, {NULL, 0, 0}};

            strcpy(vocab_set.words[vocab_set.size], tokens[j].data);
            vocab_set.size++;
        }
    }
    int **vectors = malloc(num_docs * sizeof(int *));

    if (vectors == NULL)
        return (VectorizerResult){NULL, 0, {NULL, 0, 0}};

    for (int i = 0; i < num_docs; i++)
    {
        vectors[i] = calloc(vocab_set.size, sizeof(int));

        if (vectors[i] == NULL)
            return (VectorizerResult){NULL, 0, {NULL, 0, 0}};

        struct word *tokens = word_tokenize(docs[i]);

        for (int j = 0; j < rear; j++)
        {
            int idx = find_word_jacc(
                vocab_set.words, vocab_set.size, tokens[j].data
            );

            if (idx != -1)
                vectors[i][idx]=1;
        }
    }

    VectorizerResult result = {vectors, num_docs, vocab_set};
    return result;
} 
double jaccard(char *doc1,char *doc2){

    char *docs[]={doc1,doc2};
    int common=0;
    int total=0;
    VectorizerResult result=binary_vectorizer(docs,2);

    
    for(int i=0;i<result.vocab.size;i++){
        if(result.vectors[0][i]>0 && result.vectors[1][i]>0){
            common++;
        }
        if(result.vectors[0][i]>0 || result.vectors[1][i]>0){
            total++;
        }

    }
    double similarity = (total == 0) ? 0.0 : (double)common/total;
    cleanup_vectors:
    if (result.vectors != NULL)
    {
        for (int i = 0; i < result.num_docs; i++)
            free(result.vectors[i]);

        free(result.vectors);
    }

    if (result.vocab.words != NULL)
    {
        for (int i = 0; i < result.vocab.size; i++)
            free(result.vocab.words[i]);

        free(result.vocab.words);
    }

    return similarity;

}

