#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <math.h>
#include "trainer.h"
#include "../include/libstemmer.h"

#define MAX_W 2000
#define MIN_W 2


GHashTable *neg_set;
GHashTable *non_neg_set;
int checkDel=0;
int neg_size;
int non_neg_size;
int DUMMY_W_NEG =1;
int DUMMY_W_NONNEG =1;
struct sb_stemmer *stemmer;
int common_words = 0;

const char out_csv[50] = "model.csv";
FILE *f;

GHashTable *checked;

//FILE* f_del; //TODO: remove
//FILE* f_stay; //TODO: remove

char rem_excep[10][10] = {"fate", "nogood", "unpleas", "bankrupt", "vate", "depress", "uncomfort"}; //have to remain
char del_excep[40][10] = {"to","a", "the", "he", "it", "I","they","us","am","5","26","or","didn","b","c","w","o","m","24","ve","23","2","ur","us","22","aa","ll","-",":","an","has","623","oma","dca","la","bwi","mr","ceo","dfw","lax"};                                              //have to remove

int main()
{

        f = fopen(out_csv, "w");

        //        f_del = fopen("deleted.csv", "w"); //TODO: remove
        //        f_stay = fopen("stay.csv", "w"); //TODO: remove

        stemmer = sb_stemmer_new("english", 0x0);

        neg_set = g_hash_table_new(g_str_hash, g_str_equal);
        non_neg_set = g_hash_table_new(g_str_hash, g_str_equal);

        neg_size = read_file_to_set(neg_set, "../data/train.negative.csv");
      //  printf("---------------\n");
        non_neg_size = read_file_to_set(non_neg_set, "../data/train.non-negative.csv");
        g_hash_table_foreach(neg_set, find_the_common_words, 0x0);
        //printf("neg: %d, non-neg: %d\n",neg_size,non_neg_size);

        checked = g_hash_table_new(g_str_hash, g_str_equal);
        make_model();

        sb_stemmer_delete(stemmer);
        fclose(f);

        //        fclose(f_del); //TODO: remove
        //        fclose(f_stay); //TODO: remove

        return 0;
}

int read_file_to_set(GHashTable *counter, char *file)
{

        FILE *f = fopen(file, "r");

        char *line = 0x0;
        size_t r;
        size_t n = 0;

        int size = 0;

        while (getline(&line, &n, f) >= 0)
        {
                char *t;
                char *_line = line;

                size = add_line_to_set(counter, line, size);
                free(_line);
                line = 0x0;
        }

        fclose(f);

        return size;
}

int find_the_common_words(gpointer key, gpointer value, gpointer userdata)
{
        char *word = key;
        int *d = g_hash_table_lookup(non_neg_set, word);
        if (d != NULL)
        {
                common_words++;
        }
}
int add_line_to_set(GHashTable *set, char line[], int size)
{
        char *t;

        char dili[50] = " \n\t,.#?()*&^%$@!~`/\'\"";
        char *temp = line;

        for (t = strtok(line, dili); t != 0x0; t = strtok(0x0, dili))
        {
                ; /// tokenizer has some problems

                for (int i = 0; i < strlen(t); i++)
                {
                        if (t[i] >= 'A' && t[i] <= 'Z')
                        {
                                t[i] = t[i] - 'A' + 'a';
                        }
                }

                int *d;
                const char *stem_temp = sb_stemmer_stem(stemmer, strdup(t), strlen(t));
                d = g_hash_table_lookup(set, stem_temp);
                if (d == NULL)
                {
                        d = malloc(sizeof(int));
                        *d = 1;

                        const char *stem = sb_stemmer_stem(stemmer, strdup(t), strlen(t)); // stemming
                    
                        /*  if(size > 100) exit(0);*/
                        g_hash_table_insert(set, strdup(stem), d);
                }
                else
                {
                      
                        *d = *d + 1;
                }
                size++;
        }

        return size;
}

void make_model()
{

        g_hash_table_foreach(neg_set, m1_for, 0x0);
        g_hash_table_foreach(non_neg_set, m2_for, 0x0);
}

void m1_for(gpointer key, gpointer value, gpointer userdata)
{

        char *word = key;
        int *n = value;
        /*if(strcmp(word,"self")==0){
                                        printf(" !!!!!!!!! found the word is\n");
        }*/
        int *d = g_hash_table_lookup(checked, word);

        if (d == NULL)
        {
                //  printf("abc!\n" );

                g_hash_table_insert(checked, strdup(word), -1);

                double p1, p2;

                int n2;
                int *d2 = g_hash_table_lookup(non_neg_set, word);
                if (d2 == NULL)
                        n2 = 0;
                else
                        n2 = *d2;

                int freq = *n + n2;

                if (!valid_range(word, freq))
                {
                        
                        int *t1, *t2;
                        t1 = g_hash_table_lookup(neg_set, word);
                        t2 = g_hash_table_lookup(non_neg_set, word);
                        if (t1 != NULL)
                                neg_size -= *t1;
                        if (t2 != NULL)
                                non_neg_size -= *t2;
                        return;
                }
            //    if(*n < 100 || n2) DUMMY_W = 100;
        

                p1 = (double)(*n + DUMMY_W_NEG) / (double)(neg_size + DUMMY_W_NEG);
                // p1 = (double) *n;
                if (n2 != 0)
                {
                        
                        p2 = (double)(n2 +DUMMY_W_NONNEG) / (double)(non_neg_size  + DUMMY_W_NONNEG);
                        //        p2 = log(p2);
                }
                else
                        p2 = DUMMY_W_NONNEG / (double)(non_neg_size  + DUMMY_W_NONNEG);
                //  p1 = log(p1);

        
               // double temp1 = p1;
             //   double temp2 = p2;
                p1 = log((double)p1);
                p2 = log((double)p2);
if (strcmp(word, "2") == 0 || strcmp(word, "2") == 0)
                        {
                                printf("word :  %s with %d prob : %lf prob : %lf", word, n2,p1 , p2);
                              
                        }
                fprintf(f, "%s, %lf, %lf\n", word, p1, p2);
        }

        //  g_hash_table_insert(checked, strdup(word), 0x0) ;
        else
        {

                return;
        }
}
void m2_for(gpointer key, gpointer value, gpointer userdata)
{
        char *word = key;
        int *n = value;

        int *d = g_hash_table_lookup(checked, word);
        //   int tempD=*d;
        if (d == NULL)
        {       // value not assigned for that word == if it's the new word
                //   printf("abc!\n" );

                double p1, p2;

                int n2;
                int *d2 = g_hash_table_lookup(neg_set, word);
                if (d2 == NULL)
                        n2 = 0;
                else
                        n2 = *d2;

                int freq = *n + n2;

                if (!valid_range(word, freq))
                { // if frequency is not between 10 ~ 100 it excepts it from the size

                        int *t1, *t2;
                        t1 = g_hash_table_lookup(neg_set, word);
                        t2 = g_hash_table_lookup(non_neg_set, word);

                        if (t1 != NULL)
                                neg_size -= *t1;
                        if (t2 != NULL)
                                non_neg_size -= *t2;
                        return;
                }
                // need log
              //  if(*n < 100 || n2) DUMMY_W = 100;
                p1 = (double)(*n +DUMMY_W_NONNEG) / (double)(non_neg_size + DUMMY_W_NONNEG); // adds 10 to frequency if
                if (n2 != 0)
                {
                        p2 = (double)(n2+DUMMY_W_NEG) / (double)(neg_size  + DUMMY_W_NEG); // smoothing ?
                                                                           //          p2 = log(p2);
                }
                else
                        p2 = DUMMY_W_NEG/ (double)(neg_size  + DUMMY_W_NEG);
              //  double temp1 = p1;
             //   double temp2 = p2;
                p1 = log(p1);
                p2 = log(p2);
                fprintf(f, "%s, %lf, %lf\n", word, p2, p1);

                g_hash_table_insert(checked, strdup(word), 0x0);
        }
        else
        {

                return;
        }
}
bool valid_range(char *word, int freq)
{

        if (freq < MIN_W || freq > MAX_W)
        {
                for (int i = 0; i < 10; i++)
                {
                        if (strcmp(word, rem_excep[i]) == 0)
                        {
                                //                                fprintf(f_stay, "%s, %d\n", word, freq);
                                return true;
                        }
                }
                return false;
        }
        else if (freq >= MIN_W && freq <= MAX_W)
        {
                for (int i = 0; i <40; i++)
                {
                             
                        if (strcmp(word, del_excep[i]) == 0)
                        {
                                                          //      fprintf(f_del, "%s, %d\n", word, freq);
                                return false;
                        }
                        
                }
               
                return true;
        }
    
}
