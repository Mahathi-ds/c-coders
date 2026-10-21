#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include "project.h"

int is_word_character(char ch){
    return isalnum((unsigned char)ch);
}
int is_word_equal(const char word1[],const char word2[]){
    char temp1[1000];
    char temp2[1000];
    int i;
    while (word1[i]!='\0')
    {
        temp1[i]=word1[i];
        i++;
    }
    temp1[i]='\0';
    i=0;
    while (word2[i]!='\0')
    {
        temp2[i]=word2[i];
        i++;
    }
    temp2[i]='\0';
    
    
    make_lower(temp1);
    make_lower(temp2);
    return strcmp(temp1,temp2)==0;
}
int process_file(const char *filename,const char *target_word,int *target_count,int *total_count){
    FILE *file=fopen(filename,"r");
    if(file==NULL){
        printf("error");
        return 0;
    }
    char word[1000];
    int word_length=0;
    char ch;
    
}