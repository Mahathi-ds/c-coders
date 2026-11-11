#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "project.h"
#include<stdlib.h>
#include <math.h>
#include <float.h>



int find_word(char *arr[],int size,char *word){
    for(int i=0;i<size;i++){
        if(strcmp(arr[i],word)==0){
            return i;
        }
    }
    return -1;
}

VectorizerResult vectorizer(char **docs,int num_docs){
    Vocab vocab_set;
    vocab_set.size = 0;
    vocab_set.capacity = 10;
    vocab_set.words = malloc(vocab_set.capacity * sizeof(char *));

    if (vocab_set.words == NULL)
        {return (VectorizerResult){NULL, 0, {NULL, 0, 0}};}
    for (int i = 0; i < num_docs; i++)
    {
        struct word *tokens = word_tokenize(docs[i]);

        for (int j = 0; j < rear; j++)
        {
            if (find_word(vocab_set.words, vocab_set.size,tokens[j].data) != -1)
                continue;

            if (vocab_set.size == vocab_set.capacity)
            {
                vocab_set.capacity *= 2;

                char **temp = realloc(
                    vocab_set.words,
                    vocab_set.capacity * sizeof(char *)
                );

                if (temp == NULL)
                    {return (VectorizerResult){NULL, 0, {NULL, 0, 0}};}

                vocab_set.words = temp;
            }

            vocab_set.words[vocab_set.size] =malloc(strlen(tokens[j].data) + 1);

            if (vocab_set.words[vocab_set.size] == NULL)
                return (VectorizerResult){NULL, 0, {NULL, 0, 0}};

            strcpy(vocab_set.words[vocab_set.size], tokens[j].data);
            vocab_set.size++;
        }
    }
    int **vectors = malloc(num_docs * sizeof(int *));

    if (vectors == NULL)
        return (VectorizerResult){NULL, 0, {NULL, 0, 0}};

    for (int i = 0; i < num_docs; i++)
    {
        vectors[i] = calloc(vocab_set.size, sizeof(int));

        if (vectors[i] == NULL)
            return (VectorizerResult){NULL, 0, {NULL, 0, 0}};

        struct word *tokens = word_tokenize(docs[i]);

        for (int j = 0; j < rear; j++)
        {
            int idx = find_word(
                vocab_set.words, vocab_set.size, tokens[j].data
            );

            if (idx != -1)
                vectors[i][idx]++;
        }
    }

    VectorizerResult result = {vectors, num_docs, vocab_set};
    return result;
}   


prior *class_priors(VectorizerResult result,char **labels,int num_classes){
    int size=result.num_docs;
    prior *p = calloc(num_classes, sizeof(prior));   
     if (p == NULL){
    return NULL;}
    int cap=0;
    for(int i=0;i<size;i++){
        int found=0;
        for(int j=0; j<cap;j++){
            if(strcmp(p[j].label,labels[i])==0){
                p[j].count++;
                found=1;
                break;
            }
        }
        if(found==0){
            p[cap].label=malloc(strlen(labels[i])+1);
            if (p[cap].label == NULL){

                for (int j = 0; j < cap; j++)
                    free(p[j].label);

                free(p);
                return NULL;
            }
            strcpy(p[cap].label,labels[i]);
            p[cap].count=1;
            cap++;
        }
    }
    for(int j=0;j<cap;j++){
        p[j].prob=(double)p[j].count/size;
    }
    return p;
}

DocumentGroups groupclasses(VectorizerResult result, char **labels,int num_classes){
    DocumentGroups grps;
    grps.classes = calloc(num_classes, sizeof(ClassDocuments));
    if (grps.classes == NULL){
    grps.num_classes = 0;
    return grps;
    }
    grps.num_classes=0;
    for(int i=0;i<result.num_docs;i++){
        char *label=labels[i];
        int found=0;
        int cls_idx=-1;
        for(int j=0;j<grps.num_classes;j++){
            if(strcmp( grps.classes[j].label,label)==0){
                // grps[i].classes[grps[i].num_classes].doc_indices[count]=
                found=1;
                cls_idx=j;
                break;
            }
        }
        if(found==0){
            cls_idx=grps.num_classes;
            grps.classes[cls_idx].label =malloc(strlen(label) + 1);
            if (grps.classes[cls_idx].label == NULL)
            return grps;
            strcpy(grps.classes[cls_idx].label,label);
            grps.classes[cls_idx].doc_indices = malloc(result.num_docs * sizeof(int));
            if (grps.classes[cls_idx].doc_indices == NULL)
            {
                free(grps.classes[cls_idx].label);
                grps.classes[cls_idx].label = NULL;
                return grps;
            }
            grps.classes[cls_idx].count=0;

            grps.num_classes++;


        }
        
        grps.classes[cls_idx].doc_indices[grps.classes[cls_idx].count]=i;
        grps.classes[cls_idx].count++;
        
    }

    
    return grps;
}


ClassWordCounts *build_class_word_counts(VectorizerResult result,DocumentGroups grps){
    Vocab vocab_set=result.vocab;
    ClassDocuments *grp=grps.classes;
    ClassWordCounts *arr=malloc(grps.num_classes*sizeof(ClassWordCounts));
    if(arr==NULL){
        return NULL;
    }

    for(int i=0;i<grps.num_classes;i++){
        char *class=grps.classes[i].label;
        arr[i].label = malloc(strlen(class) + 1);

        if (arr[i].label == NULL)
            return NULL;
        arr[i].word_counts = malloc(vocab_set.size * sizeof(WordFreq));
        if (arr[i].word_counts == NULL)
            return NULL;
        strcpy(arr[i].label,class);
        arr[i].size=vocab_set.size;

            
        
        for(int j=0;j<vocab_set.size;j++){
            arr[i].word_counts[j].word=vocab_set.words[j];
            int cnt=0;
            for(int k=0;k<grps.classes[i].count;k++){
                int idx=grps.classes[i].doc_indices[k];
                cnt+=result.vectors[idx][j];

            }
            arr[i].word_counts[j].count=cnt;
            

        }
    }

    
    return arr;

}


wordclasscounts *build_word_class_counts(VectorizerResult result,DocumentGroups grps,prior *priors){

    Vocab vocab_set=result.vocab;
    ClassDocuments *grp=grps.classes;
    wordclasscounts *arr=malloc(vocab_set.size*sizeof(wordclasscounts));
    if(arr==NULL){
        return NULL;
    }
    for(int i=0;i<vocab_set.size;i++){
        char *word=vocab_set.words[i];
        arr[i].word = malloc(strlen(word) + 1);
        if (arr[i].word == NULL)
            return NULL;
        arr[i].classes = malloc(grps.num_classes*sizeof());
        arr[i].size=grps.num_classes;
        if (arr[i].classes == NULL)
            return NULL;

        strcpy(arr[i].word,word);
        
        for(int j=0;j<grps.num_classes;j++){
            arr[i].classes[j].class=priors[j].label;
            int cnt=0;
            for (int k=0;k<grps.classes[j].count;k++){
                int idx=grps.classes[j].doc_indices[k];
                cnt+=result.vectors[idx][i];
            }
            arr[i].classes[j].count=cnt;

        }
    }
    return arr;
} 

char * formula(int *doc_vector, NaiveBayesModel model){
    Vocab vocab_set=model.vocab;
    int max_cls=0;
    double max_score=-DBL_MAX;
    
    for(int i=0;i<model.num_classes;i++){
        double score=log(model.priors[i].prob);
        int total_words=0;
        for(int j=0;j<vocab_set.size;j++){

            total_words+=model.class_word[i].word_counts[j].count;

        }
        for(int k=0;k<vocab_set.size;k++){
            int word_freq=model.word_class[k].classes[i].count;
            int doc_freq = doc_vector[k];

            double word_prob =(double)(word_freq + 1) /(total_words + vocab_set.size);
            score+=doc_freq*log(word_prob);
            
            
        }
        if(score>max_score){
            max_score=score;
            max_cls=i;
        }




    }
    return model.priors[max_cls].label;
}

int get_unique_classes(char **labels, int num_docs)
{
    char **classes = malloc(num_docs * sizeof(char *));

    if (classes == NULL)
        return -1;

    int count = 0;

    for (int i = 0; i < num_docs; i++){
        int found = 0;
        for (int j = 0; j < count; j++)
        {
            if (strcmp(labels[i], classes[j]) == 0){
                found = 1;
                break;
            }
        }

        if (found == 0)
        {
            classes[count] = labels[i];
            count++;
        }
    }

    free(classes);
    return count;
}
int **vectorizer_transform(char **docs, int num_docs, Vocab vocab){

    int **vectors = malloc(num_docs * sizeof(int *));

    if (vectors == NULL)
        return NULL;

    for (int i = 0; i < num_docs; i++)
    {
        vectors[i] = calloc(vocab.size, sizeof(int));

        if (vectors[i] == NULL)
            return NULL;
        for (int i = 0; i < num_docs; i++){
        vectors[i] = calloc(vocab.size, sizeof(int));
        if (vectors[i] == NULL)
        {for (int j = 0; j < i; j++){
                free(vectors[j]);}

            free(vectors);
            return NULL;
        }

        struct word *tokens = word_tokenize(docs[i]);

        for (int j = 0; j < rear; j++)
        {
            int idx = find_word(vocab.words, vocab.size, tokens[j].data);

            if (idx != -1)
                vectors[i][idx]++;
        }
    }
}

    return vectors;
}


NaiveBayesModel Naivebayes_fit(char **docs,char **labels,int num_docs){

    NaiveBayesModel model={0};
    if (docs == NULL || labels == NULL || num_docs <= 0)
        return model;
    VectorizerResult result=vectorizer(docs,num_docs);
    if (result.vocab.words == NULL || result.vectors == NULL)
    return (NaiveBayesModel){0};
    int num_classes=get_unique_classes(labels,num_docs);
    DocumentGroups grps=groupclasses(result,labels,num_classes);
    if (grps.classes == NULL || grps.num_classes != num_classes)
        goto cleanup_groups;
    model.priors=class_priors(result,labels,num_classes);
    if (model.priors == NULL)
        goto cleanup_groups;

    model.class_word = build_class_word_counts(result, grps);

    if (model.class_word == NULL)
        goto cleanup_priors;

    model.word_class = build_word_class_counts(result, grps,model.priors);

    if (model.word_class == NULL)
        goto cleanup_class_word;

    model.vocab = result.vocab;
    model.num_classes = grps.num_classes;
    for (int i = 0; i < result.num_docs; i++)
        free(result.vectors[i]);

    free(result.vectors);

    for (int i = 0; i < grps.num_classes; i++){
        free(grps.classes[i].label);
        free(grps.classes[i].doc_indices);}

    free(grps.classes);

    return model;

cleanup_class_word:
    for (int i = 0; i < grps.num_classes; i++)
    {
        free(model.class_word[i].label);
        free(model.class_word[i].word_counts);
    }
    free(model.class_word);


cleanup_priors:
    for (int i = 0; i < num_classes; i++)
        free(model.priors[i].label);

    free(model.priors);

cleanup_groups:
    if (grps.classes != NULL)
    {
        for (int i = 0; i < grps.num_classes; i++)
        {
            free(grps.classes[i].label);
            free(grps.classes[i].doc_indices);
        }
        free(grps.classes);
    }

cleanup_vectors:
    if (result.vectors != NULL)
    {
        for (int i = 0; i < result.num_docs; i++)
            free(result.vectors[i]);

        free(result.vectors);
    }

    if (result.vocab.words != NULL)
    {
        for (int i = 0; i < result.vocab.size; i++)
            free(result.vocab.words[i]);

        free(result.vocab.words);
    }

    return (NaiveBayesModel){0};
}






char **Naivebayes_predict(NaiveBayesModel model,char **test_docs,int num_docs)
{
    if (test_docs == NULL || num_docs <= 0 ||
        model.vocab.words == NULL || model.num_classes <= 0)
        return NULL;
    int **vectors = vectorizer_transform(test_docs, num_docs, model.vocab);

    if (vectors == NULL)
        return NULL;

    char **predictions = malloc(num_docs * sizeof(char *));

    if (predictions == NULL)
    {
        for (int i = 0; i < num_docs; i++)
            free(vectors[i]);

        free(vectors);
        return NULL;
    }

    for (int i = 0; i < num_docs; i++){
        char *label = formula(vectors[i], model);

        predictions[i] = malloc(strlen(label) + 1);

        if (predictions[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(predictions[j]);

            free(predictions);

            for (int j = 0; j < num_docs; j++)
                free(vectors[j]);

            free(vectors);
            return NULL;
        }
        strcpy(predictions[i], label);
    }

    // Free the transformed vectors
    for (int i = 0; i < num_docs; i++)
        free(vectors[i]);

    free(vectors);

    return predictions;
}


void NaiveBayes_free(NaiveBayesModel *model)
{
    if (model == NULL)
        return;

    // Free vocabulary
    for (int i = 0; i < model->vocab.size; i++)
        free(model->vocab.words[i]);

    free(model->vocab.words);

    // Free priors and their labels
    for (int i = 0; i < model->num_classes; i++)
        free(model->priors[i].label);

    free(model->priors);

    // Free class_word
    for (int i = 0; i < model->num_classes; i++) {
        free(model->class_word[i].label);
        free(model->class_word[i].word_counts);
    }

    free(model->class_word);

    // Free word_class
    for (int i = 0; i < model->vocab.size; i++) {
        free(model->word_class[i].word);
        free(model->word_class[i].classes);
    }

    free(model->word_class);

    // Reset model
    *model = (NaiveBayesModel){0};
}







