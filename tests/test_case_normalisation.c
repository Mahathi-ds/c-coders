#include<stdio.h>
#include <assert.h>
#include<string.h>
#include "project.h"
void test_spl_chars(){
    char text[]="HelLo@123 WORLD";
    make_lower(text);
    assert(strcmp(text, "hello@123 world") == 0);
    printf("success");

}
int main(void){
       test_spl_chars();
       return 0;
}