
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project.h"

void test_normal() {
    char text[] = "Hello World";
    struct word **lst = word_tokenize(text);
    printf("\nNormal case:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",lst[i]->data);
    }
}

void test_punctuation() {
    char text1[] = "Hello, World!";
    struct word **lst = word_tokenize(text1);
    printf("\nPunctuation case:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",lst[i]->data);
    }
}

void test_digits() {
    char text2[] = "NLP123";
    struct word **lst = word_tokenize(text2);
    printf("\nDigits case:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",lst[i]->data);
    }
}

void test_spaces() {
    char text[] = "Hello    World";
    struct word **lst = word_tokenize(text);
    printf("\nMultiple spaces:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",lst[i]->data);
    }
}

void test_empty() {
    char text[] = "";
    word_tokenize(text);
    printf("\nEmpty input: Token count = %d\n",rear);
    if(rear==0){
        printf("PASS\n");
    }
    else{
        printf("FAIL\n");
    }
}

void test_null() {
    struct word **lst = word_tokenize(NULL);
    printf("\nNULL input: %s\n",
           lst == NULL ? "Handled" : "Not handled");
}

void test_single_character() {
    char text[] = "A";
    struct word **lst = word_tokenize(text);
    printf("\nSingle character:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",lst[i]->data);
    }
}

void test_large_input() {
    char text[6000] = "";
    for(int i=0; i<900; i++){
        strcat(text,"Hello ");
    }
    struct word **lst = word_tokenize(text);
    printf("\nLarge input:\n");
    printf("Expected tokens: 900\n");
    printf("Actual tokens: %d\n",rear);
    if(lst!=NULL && rear==900){
        printf("PASS");
    }
    else{
        printf("FAIL\n");
    }
}

void test_only_punctuation() {
    char text[] = "!!!";
    struct word **lst = word_tokenize(text);
    printf("\nOnly punctuation:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",lst[i]->data);
    }
}

int main(void) {

    test_normal();
    test_punctuation();
    test_digits();
    test_spaces();
    test_empty();
    test_null();
    test_single_character();
    test_large_input();
    test_only_punctuation();

    return 0;
}
