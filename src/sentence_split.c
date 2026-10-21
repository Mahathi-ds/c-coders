#include<stdio.h>
#include<ctype.h>
#include "project.h"

int sentence_split(char *text,char sentences[][500],int max_sentences){
	int i=0;
	int j=0;
	int sentence_count=0;
	char ch;
	if(text==NULL||max_sentences<=0){
		return 0;
}
	while(text[i]!='\0'&&sentence_count<max_sentences){

		ch=text[i];
		sentences[sentence_count][j]=ch;
		j++;
		if(ch=='.'||ch=='!'||ch=='?'){
			sentences[sentence_count][j]='\0';
			sentence_count++;
			j=0;
			i++;
			while(text[i]!='\0' && isspace((unsigned char)text[i])){
	i++;
	
}
continue;
}
i++;

}
if(j>0 && sentence_count<max_sentences){
	sentences[sentence_count][j]='\0';
	sentence_count++;
}
return sentence_count;

}