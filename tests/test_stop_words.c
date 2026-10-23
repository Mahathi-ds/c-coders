
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project.h"
void test_normal() {
    char words[] = "This is a beautiful day";

    printf("\nTest 1 - Normal case\n");
    printf("Input : %s\n", words);

    struct word **lst = word_tokenize(words);
    if(lst == NULL) {
        printf("Output: Tokenization failed\n");
        return;
    }

    int status = remove_stopwords(lst);
    printf("Output: ");
    if(status == 0) {
        for(int i=0; i<rear; i++){
            printf("%s ", lst[i]->data);
        }
        printf("\n");
    }
    else {
        printf("Stop-word removal failed\n");
    }
}

void test_only_stopwords() {
    char words[] = "i was doing it for them";
    printf("\nTest 2 - Only stopwords\n");
    printf("Input : %s\n", words);
    struct word **lst = word_tokenize(words);
    if(lst == NULL) {
        printf("Output: Tokenization failed\n");
        return;
    }

    int status = remove_stopwords(lst);

    printf("Output: Token count = %d\n", rear);
    printf("Expected token count: 0\n");

    if(status == 0 && rear == 0)
        printf("Result: PASS\n");
    else
        printf("Result: FAIL\n");
}

void test_no_stopwords() {
    char words[] = "cat dog elephant";

    printf("\nTest 3 - No stopwords\n");
    printf("Input : %s\n", words);

    struct word **lst = word_tokenize(words);
    if(lst == NULL) {
        printf("Output: Tokenization failed\n");
        return;
    }

    int status = remove_stopwords(lst);

    printf("Output: ");
    if(status == 0) {
        for(int i=0; i<rear; i++){
            printf("%s ", lst[i]->data);
        }
        printf("\n");
    }
    else {
        printf("Stop-word removal failed\n");
    }

    printf("Expected token count: 3\n");
    printf("Actual token count: %d\n", rear);

    if(status == 0 && rear == 3)
        printf("Result: PASS\n");
    else
        printf("Result: FAIL\n");
}

void test_empty() {
    char words[] = "";

    printf("\nTest 4 - Empty input\n");
    printf("Input : %s\n", words);

    struct word **lst = word_tokenize(words);

    int status = remove_stopwords(lst);

    printf("Output: Token count = %d\n", rear);
    printf("Removal status: %d\n", status);
    if(rear == 0 && status == -1)
        printf("Result: PASS\n");
    else
        printf("Result: FAIL\n");
}

void test_repeated() {
    char words[] = "the the cat is is sleeping";

    printf("\nTest 5 - Repeated stopwords\n");
    printf("Input : %s\n", words);

    struct word **lst = word_tokenize(words);
    if(lst == NULL) {
        printf("Output: Tokenization failed\n");
        return;
    }

    int status = remove_stopwords(lst);

    printf("Output: ");
    if(status == 0) {
        for(int i=0; i<rear; i++){
            printf("%s ", lst[i]->data);
        }
        printf("\n");
    }
    else {
        printf("Stop-word removal failed\n");
    }

    printf("Expected: cat sleeping\n");

    if(status == 0 && rear == 2 &&
       strcmp(lst[0]->data, "cat") == 0 &&
       strcmp(lst[1]->data, "sleeping") == 0)
        printf("Result: PASS\n");
    else
        printf("Result: FAIL\n");
}

void test_large_input() {
    char words[4000] = "";

    for(int i=0; i<890; i++){
        strcat(words, "the ");
    }
    for(int i=890;i<900;i++){
        strcat(words,"cat ");
    }

    printf("\nTest 6 - Large input\n");
    printf("Input : 890 repetitions of \"the \" and 10 repetitions of \"cat \"\n");
    struct word **lst = word_tokenize(words);
    if(lst == NULL) {
        printf("Output: Tokenization failed\n");
        return;
    }

    int status = remove_stopwords(lst);

    printf("Output: Token count = %d\n", rear);
    printf("Expected token count: 10\n");

    if(status == 0 && rear == 10)
        printf("Result: PASS\n");
    else
        printf("Result: FAIL\n");
}

int main(void) {

    printf("========== STOP-WORD REMOVAL TESTS ==========\n");

    test_normal();
    test_only_stopwords();
    test_no_stopwords();
    test_empty();
    test_repeated();
    test_large_input();


    return 0;
}
