
#include <stdio.h>
#include <string.h>
#include "project.h"


void test_normal() {
    char words[] = "This is a beautiful day";
    struct word **lst = word_tokenize(words);   
    remove_stopwords(lst);
    printf("\nNormal case:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",lst[i]->data);
    }
}

void test_only_stopwords() {
    char words[] = "i was doing it for them";
    struct word **lst = word_tokenize(words);
    remove_stopwords(lst);
    printf("\nOnly stopwords:\n");
    printf("Expected tokens: 0\n");
    printf("Actual tokens: %d\n",rear);
    if(rear == 0)
        printf("PASS\n");
    else
        printf("FAIL\n");
}

void test_no_stopwords() {
    char words[] = "cat dog elephant";
    struct word **lst = word_tokenize(words);
    remove_stopwords(lst);
    printf("\nNo stopwords:\n");
    if(rear == 3)
        printf("PASS\n");
    else
        printf("FAIL\n");
}

void test_empty() {
    char words[] = "";
    struct word **lst = word_tokenize(words);
    rear = 0;

    remove_stopwords(lst);
    printf("\nEmpty input:\n");

    if(rear == 0)
        printf("PASS\n");
    else
        printf("FAIL\n");
}

void test_repeated() {
    char words[] = "the the cat is is sleeping";
    struct word **lst = word_tokenize(words);
    remove_stopwords(lst);
    printf("\nRepeated stopwords:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",lst[i]->data);
        }
    }

void test_large_input() {
    char words[4000] = "";

    for(int i = 0; i < 900; i++){
        strcat(words, "the ");
    }
    struct word **lst = word_tokenize(words);
    remove_stopwords(lst);

    printf("\nLarge input:\n");
    printf("Expected tokens: 0\n");
    printf("Actual tokens: %d\n",rear);

    if(rear == 0)
        printf("PASS\n");
    else
        printf("FAIL\n");
}

int main() {
    test_normal();
    test_only_stopwords();
    test_no_stopwords();
    test_empty();
    test_repeated();
    test_large_input();

    return 0;
}
