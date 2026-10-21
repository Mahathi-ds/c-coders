#include<stdio.h>
#include<string.h>
#include <stdlib.h>

char lower[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char upper[]={'A','B','C','D','E','F' ,'G', 'H' ,'I', 'J', 'K', 'L', 'M' ,'N' ,'O', 'P', 'Q' ,'R', 'S', 'T', 'U', 'V', 'W', 'X' ,'Y', 'Z'};

void make_lower(char text[]){
    int n=strlen(text);
    for(int i=0;i<n;i++){
        for(int j=0;j<26;j++){
            if(upper[j]==text[i]){
                text[i]=lower[j];
                break;
            }
           

        }
        

    }
  
}

void make_upper(char text[]){
    int n=strlen(text);
    for(int i=0;i<n;i++){
        for(int j=0;j<26;j++){
            if(lower[j]==text[i]){
                text[i]=upper[j];
                break;
            }
        }
    }
        

}



int main(void){
    char text[] = "MahAthi is GOOD";
    make_lower(text);
    make_upper(text);
    
    

    for(int i=0;i<strlen(text);i++){
        printf("%c",text[i]);

    }
    return 0;


}