#include<stdio.h>
#include <assert.h>
#include<string.h>
#include "project.h"
#include<stdlib.h>

void check_result(char name[], char expected[], char actual[]) {

    printf("\nTest Case : %s\n", name);
    printf("Expected  : [%s]\n", expected);
    printf("Actual    : [%s]\n", actual);

    if(strcmp(expected, actual) == 0)
        printf("Result    : PASS\n");
    else
        printf("Result    : FAIL\n");
}

void test_normal_lower() {
    char text[] = "HeLLo WoRLd";
    make_lower(text);

    check_result("Normal Lowercase", "hello world", text);
}

void test_normal_upper() {
    char text[] = "HeLLo WoRLd";
    make_upper(text);

    check_result("Normal Uppercase", "HELLO WORLD", text);
}

void test_already_lower() {
    char text[] = "hello";
    make_lower(text);

    check_result("Already Lowercase", "hello", text);
}

void test_already_upper() {
    char text[] = "HELLO";
    make_upper(text);

    check_result("Already Uppercase", "HELLO", text);
}

void test_mixed() {
    char text[] = "HeLLo123!@#";
    make_lower(text);
    check_result("Mixed Characters", "hello123!@#", text);
}


void test_single_character() {
    char text[] = "A";
    make_lower(text);

    check_result("Single Character", "a", text);
}

void test_empty() {
    char text[] = "";
    make_lower(text);

    check_result("Empty Input", "", text);
}

void test_null() {
    int status1 = make_lower(NULL);
    int status2 = make_upper(NULL);

    printf("\nTest Case : NULL Input\n");
    printf("Expected  : -1 for both functions\n");
    printf("Actual    : lower=%d, upper=%d\n", status1, status2);

    if(status1 == -1 && status2 == -1)
        printf("Result    : PASS\n");
    else
        printf("Result    : FAIL\n");
}

void test_large_input() {
    int n = 10000;
    char *text = malloc(n + 1);

    if(text == NULL) {
        printf("\nTest Case : Large Input\n");
        printf("Result    : FAIL (Memory allocation failed)\n");
        return;
    }

    memset(text, 'A', n);
    text[n] = '\0';

    int status = make_lower(text);
    int correct = (status == 0);

    for(int i=0; i<n; i++) {
        if(text[i] != 'a') {
            correct = 0;
            break;
        }
    }
    printf("\nTest Case : Large Input (10000 characters)\n");
    printf("Expected  : All lowercase a\n");
    printf("Actual    : First 30 characters = %.30s\n", text);
    printf("Result    : %s\n", correct ? "PASS" : "FAIL");

    free(text);
}
void test_non_alphabetic() {
    char text[] = "12345!@#$%";
    make_lower(text);

    check_result("Non-Alphabetic Input", "12345!@#$%", text);
}

int main(void) {

    printf("====================================\n");
    printf("     CASE NORMALISATION TESTING\n");
    printf("====================================\n");

    test_normal_lower();
    test_normal_upper();
    test_already_lower();
    test_already_upper();
    test_mixed();
    test_single_character();
    test_empty();
    test_null();
    test_large_input();
    test_non_alphabetic();

    printf("\n====================================\n");
    printf("          TESTING COMPLETED\n");
    printf("====================================\n");

    return 0;
}
