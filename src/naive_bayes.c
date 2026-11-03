#include<stdio.h>
#include<string.h>
#include "project.h"


int find_word(Vocab vocab,char *word){
    for(int i=0;i<vocab.size;i++){
        if(strcmp(vocab.words[i],word)==0){
            return 1;
        }
    }
    return 0;
}

VectorizerResult vectorizer(char **docs,int num_docs){
    Vocab vocab;
    vocab.size=0;
    vocab.capacity=10;
    vocab.words=malloc(vocab.capacity*sizeof(char));
    for(int i=0;i<num_docs;i++){
        char *doc=docs[i];
        for(int j=0;j<strlen(doc);j++){
            if(find_word(vocab,doc[j])==1){
                continue;
            }
            else{
                vocab.words[vocab.size]=doc[j];
                if(vocab.size<vocab.capacity){
                    vocab.size++;
                }
                else if (vocab.size == vocab.capacity) {
                        vocab.capacity *= 2;

                        char **temp = realloc(vocab.words,vocab.capacity * sizeof(char *));

                        if (temp == NULL) {
                            
                        }
                        else {
                            vocab.words = temp;
                        }
                    }

                }
            }


        }
    }
    

}

