
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project.h"

void test_normal() {
    char text[] = "Hello World";

    printf("\nTest 1 - Normal case\n");
    printf("Input : %s\n", text);

    struct word **lst = word_tokenize(text);

    printf("Output: [");
    for(int i=0; lst!=NULL && i<rear; i++){
        printf("\"%s\"", lst[i]->data);
        if(i<rear-1)
            printf(", ");
    }
    printf("]\n");
}

void test_punctuation() {
    char text[] = "Hello, World!";

    printf("\nTest 2 - Punctuation\n");
    printf("Input : %s\n", text);

    struct word **lst = word_tokenize(text);

    printf("Output: [");
    for(int i=0; lst!=NULL && i<rear; i++){
        printf("\"%s\"", lst[i]->data);
        if(i<rear-1)
            printf(", ");
    }
    printf("]\n");
}

void test_digits() {
    char text[] = "NLP123";

    printf("\nTest 3 - Digits\n");
    printf("Input : %s\n", text);

    struct word **lst = word_tokenize(text);

    printf("Output: [");
    for(int i=0; lst!=NULL && i<rear; i++){
        printf("\"%s\"", lst[i]->data);
        if(i<rear-1)
            printf(", ");
    }
    printf("]\n");
}

void test_spaces() {
    char text[] = "Hello    World";

    printf("\nTest 4 - Multiple spaces\n");
    printf("Input : %s\n", text);

    struct word **lst = word_tokenize(text);

    printf("Output: [");
    for(int i=0; lst!=NULL && i<rear; i++){
        printf("\"%s\"", lst[i]->data);
        if(i<rear-1)
            printf(", ");
    }
    printf("]\n");
}

void test_empty() {
    char text[] = "";

    printf("\nTest 5 - Empty input\n");
    printf("Input : %s\n", text);

    struct word **lst = word_tokenize(text);

    printf("Output: []\n");
    printf("Token count: %d\n", rear);

    if(lst!=NULL && rear==0)
        printf("Result: PASS\n");
    else
        printf("Result: FAIL\n");
}

void test_null() {
    printf("\nTest 6 - NULL input\n");
    printf("Input : NULL\n");

    struct word **lst = word_tokenize(NULL);

    if(lst==NULL)
        printf("Output: NULL returned safely\n");
    else
        printf("Output: Unexpected result\n");
}

void test_single_character() {
    char text[] = "A";

    printf("\nTest 7 - Single character\n");
    printf("Input : %s\n", text);

    struct word **lst = word_tokenize(text);

    printf("Output: [");
    for(int i=0; lst!=NULL && i<rear; i++){
        printf("\"%s\"", lst[i]->data);
        if(i<rear-1)
            printf(", ");
    }
    printf("]\n");
}

void test_large_input() {
    char text[6000] = "";

    for(int i=0; i<900; i++){
        strcat(text, "Hello ");
    }

    printf("\nTest 8 - Large input\n");
    printf("Input : 900 repetitions of \"Hello \"\n");

    struct word **lst = word_tokenize(text);

    printf("Output: List containing %d tokens\n", rear);
    printf("Expected token count: 900\n");
    printf("Actual token count: %d\n", rear);

    if(lst!=NULL && rear==900)
        printf("Result: PASS\n");
    else
        printf("Result: FAIL\n");
}

void test_only_punctuation() {
    char text[] = "!!!";

    printf("\nTest 9 - Only punctuation\n");
    printf("Input : %s\n", text);

    struct word **lst = word_tokenize(text);

    printf("Output: [");
    for(int i=0; lst!=NULL && i<rear; i++){
        printf("\"%s\"", lst[i]->data);
        if(i<rear-1)
            printf(", ");
    }
    printf("]\n");
}

int main(void) {

    printf("========== WORD TOKENIZATION TESTS ==========\n");

    test_normal();
    test_punctuation();
    test_digits();
    test_spaces();
    test_empty();
    test_null();
    test_single_character();
    test_large_input();
    test_only_punctuation();

    printf("\n========== TESTING COMPLETED ==========\n");

    return 0;
}
