/*#ifndef _predictor_h
#define _predictor_h

int read_model_neg(GHashTable * counter,char* file) ;
int read_model_nonneg(GHashTable * counter,char* file) ;
void p_counter (gpointer key, gpointer value, gpointer userdata);
void checkingHash()
#endif

*/
#ifndef _predictor_h
#define _predictor_h

int read_model(GHashTable * neg, GHashTable * nonneg, char* file) ;
void p_counter (gpointer key, gpointer value, gpointer userdata);
void checkingHash();
void read_csv();
double predict_result(char* set_of_file,GHashTable * neg_hash, GHashTable * non_neg_hash,double result,int flag);
#endif