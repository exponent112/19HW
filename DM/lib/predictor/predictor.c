#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <math.h>
#include "predictor.h"
#include "../../include/libstemmer.h"

GHashTable * model_neg;
GHashTable * model_nonneg;
GHashTable * message_set;
struct sb_stemmer * stemmer ;
int cntNON =0;
int cntNEG =0;
int model_size;
int cnt=0;
#define MAX_LEN 102400
int main (){
    stemmer = sb_stemmer_new("english", 0x0) ;
    
    model_neg = g_hash_table_new(g_str_hash, g_str_equal);
    model_nonneg = g_hash_table_new(g_str_hash, g_str_equal);
  //  predict_result(read_csv());
    model_size = read_model(model_neg, model_nonneg, "model.csv");
    read_csv();
 
    sb_stemmer_delete(stemmer);
   // predict_result();
}


void read_csv(){
    FILE * f = fopen("../../data/test.negative.csv", "r") ;
 //   FILE * f2 = fopen("neg_data.csv", "w") ;
  //  FILE * f3 = fopen("nonneg_data.csv", "w") ;
    char** set_of_file;
    char s_file[1024];
    int t = 0;
    while(!feof(f)){
        int count  = 0;
        fgets(s_file,1024,f);
      
        set_of_file = (char**) malloc ( sizeof(char*) * 1024 );
        for(int i=0; i<1024; i++){
            set_of_file[i] = (char*) malloc ( sizeof(char) * 1024);
        }

        //배열 저장 
        char *word_f;
        word_f = strtok(s_file, " \n\t,.#?()*&^%$@!~`/\'\"");
         while(word_f != NULL ){
             for(int i = 0; i < strlen(word_f); i++) {
                                if(word_f[i] >= 'A' && word_f[i] <= 'Z') {
                                        word_f[i] = word_f[i] - 'A' + 'a';
                                }
                        }
                const char* stem = sb_stemmer_stem(stemmer, strdup(word_f), strlen(word_f)) ;
                
                set_of_file[count] = strdup(stem);
                
                count++;
                word_f = strtok(NULL, " \n\t,.#?()*&^%$@!~`/\'\"");

        }
        
          double result_neg = 0.0;
          double result_nonneg =0.0;
           double * d_neg ;
        double * d_nonneg;
      for(int i=0;i<count;i++){
        /* result_neg =  predict_result(set_of_file[i],model_neg,model_nonneg,result_neg,0);
         result_nonneg = predict_result(set_of_file[i],model_neg,model_nonneg,result_nonneg,1);*/
        d_neg = g_hash_table_lookup(model_neg, set_of_file[i]) ;
        d_nonneg = g_hash_table_lookup(model_nonneg, set_of_file[i]) ;
        //if(!strcmp(set_of_file[i],"profits"))
       // exit(0);
        if(d_neg != NULL){
           
            if(*d_nonneg<0)result_neg = result_neg + *d_neg;
        {
                printf("word : %s probability neg %lf \n", set_of_file[i],*d_neg);
        }
        }
        
        if(d_nonneg!=NULL){
         
            if(*d_neg<0)result_nonneg = result_nonneg + *d_nonneg;
          //  else result_neg = result_neg - *d_neg;
            printf("word : %s probability nonneg %lf \n", set_of_file[i],*d_nonneg);
        }
      }
      free(set_of_file);
       printf("pred1 %f prediction2 %f\n",result_neg,result_nonneg);
     
      double prediction = result_neg / (result_nonneg + result_neg);
      prediction = 1 - prediction;
       // free(set_of_file);
       cnt++;
        printf("line %d prediction %f\n",t,prediction);
      
      
        if(prediction > 0.50) {
            printf("\n !!!!!!! NEGATIVE !!!!!!!! \n");
          //    fprintf(f2,"line %d prediction %f\n",t,prediction);
            cntNEG++;
        }
        else{
        //      fprintf(f3,"line %d prediction %f\n",t,prediction);
printf("\n !!!!!!! NON NEGATIVE !!!!!!!! \n");
cntNON++;
        } 
        t++;
    }
    printf("\n neg : %d     nonneg %d \n",cntNEG,cntNON);
    //return set_of_file;
}
void p_counter(gpointer key, gpointer value, gpointer userdata)
{
    char *t = key;
    double *d = value;
    printf("(%s %lf)\n", t, *d);
}
void checkingHash()
{

    g_hash_table_foreach(model_neg, p_counter, 0x0);
    //     g_hash_table_foreach(non_neg_set, p_counter, 0x0);
}
int read_model(GHashTable * neg, GHashTable * nonneg, char* file) {
    
    char word[MAX_LEN];
    double * num1 = malloc(sizeof(double));
    double * num2 = malloc(sizeof(double));

    FILE * f = fopen(file, "r") ;

    int size = 0;
    while (!feof(f)) {
        fscanf(f,"%[^,], %lf, %lf ",word,num1,num2);
    //    printf("%s, %lf, %lf\n",word,*num1,*num2);
        //double temp1 = *num1;
       // double temp2 = *num2;
        g_hash_table_insert(neg, strdup(word),strdup(num1));
        g_hash_table_insert(nonneg, strdup(word), strdup(num2));
        size++;
    }

    fclose(f);

    return size;
}

