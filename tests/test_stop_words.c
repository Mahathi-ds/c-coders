
#include <stdio.h>
#include <string.h>
#include "project.h"


void test_normal() {
    char text[] = "This is a beautiful day";
    char *words[] = {"This", "is", "a", "beautiful", "day"};   
    remove_stopwords(words);
    printf("\nNormal case:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",words[i]);
    }
}

void test_only_stopwords() {
    char *words[] = {"the", "is", "a", "an", "and"};
    remove_stopwords(words);
    printf("\nOnly stopwords:\n");
    printf("Expected tokens: 0\n");
    printf("Actual tokens: %d\n",rear);
    if(rear == 0)
        printf("PASS\n");
    else
        printf("FAIL\n");
}

void test_no_stopwords() {
    char *words[] = {"cat", "dog", "elephant"};
    remove_stopwords(words);
    printf("\nNo stopwords:\n");
    if(rear == 3)
        printf("PASS\n");
    else
        printf("FAIL\n");
}

void test_empty() {
    struct word *lst[1] = {NULL};
    rear = 0;

    remove_stopwords(lst);
    printf("\nEmpty input:\n");

    if(rear == 0)
        printf("PASS\n");
    else
        printf("FAIL\n");
}

void test_repeated() {
    char *words[] = {"the", "the", "cat", "is", "is", "sleeping"};
    remove_stopwords(words);
    printf("\nRepeated stopwords:\n");
    for(int i=0; i<rear; i++){
        printf("%s\n",words[i]);
        }
    }

void test_large_input() {
    char *words[900];

    for(int i=0; i<900; i++){
        words[i] = "the";
    }
   

    remove_stopwords(words);

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
