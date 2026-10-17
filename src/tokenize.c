#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char punct[]={'!','?','.',',','/',':',';','@','#','%','$'};
char digits[]={'1','2','3','4','5','6','7','8','9','0'};

struct word{
    char data[1000];
    int idx;
};
struct word *stack[1000];  /*array of structs word*/
int rear=0;
int n=sizeof(punct);

int is_punct(char ch){
    int n=sizeof(punct)/sizeof(punct[0]);

    for(int k=0;k<n;k++){
        if(punct[k]==ch){
            return 1;
        }

    }
    return 0;
}
int is_digit(char ch){
    int n=sizeof(digits)/sizeof(digits[0]);

    for(int k=0;k<n;k++){
        if(digits[k]==ch){
            return 1;
        }

    }
    return 0;
}


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
        return ;
    }
    int i=0;
    while(i<strlen(text)){
        
        int j=i;
        if(text[j]==' '){
            i++;
            continue;
            
        }
        struct word *new=create();
        while(text[j]!=' '  && text[j]!='\0' && !is_punct(text[j]) && !is_digit(text[j]) ){
            

            printf("checking %c\n",text[j]);
            
            new->data[new->idx]=text[j];
            new->idx++;
            j++;

        }
        if(new->idx>0){
        new->data[new->idx]='\0';
        stack[rear]=new;
        rear++;}
        if(is_punct(text[j]) || is_digit(text[j])){
            if(is_digit(text[j])){
            printf("encountered digit %c\n",text[j]);}
            else{printf("encountered punct %c\n",text[j]);}


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

int main(void){
    struct word **lst=word_tokenize("Hello, World! I'm learning NLP (version 2.0); isn't it amazing? My email is test123@gmail.com.");
    for(int i=0;i<rear;i++){
        printf("%s\n",lst[i]->data);
    }
    return 0;

}
