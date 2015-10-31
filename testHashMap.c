#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>
#include "def.h"
#include "nbhash.h"
#define NUM_THREADS 512
#define N 10
int main(){

    Hashtable_t * ht = ht_newHashTable(10);
    #pragma omp parallel num_threads(NUM_THREADS)
    {
      int tid = omp_get_thread_num()+1;
      int ee = 1;
      int*e = &ee;
      //Key_t k = 2;

      int i;
      for(i=0;i<N;i++){
        ht_put(ht,tid+i,3*tid,e);
      if(*e != NO_ERROR){
        printf("fail to put! error, errorno = %d\n",*e);
          
          }
      }
      


    }

    ht_print(ht);
    int slots = ht_claimedSlots(ht);
    printf("claimed slots: %d\n",slots);





   //  Key_t k1 = 1;
   //  Val_t v1 = 2;
   //  int temp = 1;
   //  int* e = &temp;
   //  ht_put(ht,k1,v1,e);
   //  if(*e != NO_ERROR){
   //      printf("fail to put! error\n");
   //      goto final;
   //  }
   // Val_t val = ht_get(ht,k1,e);
   // if(*e != NO_ERROR)
   // {
   //     puts("get fail!\n");
   //     goto final;
   // }
   // printf("val=%u\n",val);






    ht_freeHashTable(ht);

    return 0;


    

}
