#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include "project.h"



struct word *stack[1000];  /*array of structs word*/
int rear=0;

struct word *create(){
    struct word *new=malloc(sizeof(struct word));
    if(new==NULL){
        printf("Memory not allocated");
    }
    new->idx=0;
    return new;
}

struct word **word_tokenize(char text[]){
    if(text==NULL){
        printf("Empty text");
        return NULL;
    }
    rear=0;
    int i=0;
    while(i<strlen(text)){
        
        int j=i;
        if(isspace(text[j]==0)){
            i++;
            continue;
            
        }
        struct word *new=create();
        while(text[j]!=' '  && text[j]!='\0' && !ispunct(text[j]) && !isdigit(text[j]) ){
                    
            new->data[new->idx]=text[j];
            new->idx++;
            j++;

        }
        if(new->idx>0){
        new->data[new->idx]='\0';
        stack[rear]=new;
        rear++;}
        if(ispunct(text[j]) || isdigit(text[j])){
            


            struct word *token=create();
            token->data[token->idx]=text[j];
            token->idx++;
            token->data[token->idx]='\0';
            stack[rear]=token;
            rear++;
            
        }
        
        i=j+1;
        

    }
    return stack;


}


