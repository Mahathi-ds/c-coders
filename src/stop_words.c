#include<stdio.h>
char stop_words(){


FILE *fp=fopen("C:/Users/ctrl2/Downloads/NLTK's list of english stopwords","r");
if(fp==NULL){
    printf("not opened");
    return 1;
}

char stopwords[1000][100];
int count=0;

while (count<1000 && fscanf(fp, "%s", stopword[count]) == 1) {
    count++;
}
fclose(fp);

return stopwords;
}

int main(struct word *tokens[]){

    for(int i=0;i<size(tokens)/sizeof(tokens[0]);i++){
        for(int j=0;j<sizeof(stopwords);j++){
            if(tokens[i]==stopwords[j]){
                tokens
            }
        }
    }

}