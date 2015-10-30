#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include <omp.h>
#include "def.h"
#include "nbhash.h"
#define NUM_THREADS 8
#define N 100
int main(){

    Hashtable_t * ht = ht_newHashTable(10);
    Key_t k1 = 1;
    Val_t v1 = 2;
    int temp = 1;
    int* e = &temp;
    ht_put(ht,k1,v1,e);
    if(*e != NO_ERROR){
        printf("fail to put! error\n");
        goto final;
    }
   Val_t val = ht_get(ht,k1,e);
   if(*e != NO_ERROR)
   {
       puts("get fail!\n");
       goto final;
   }
   printf("val=%u\n",val);






    final:
    ht_freeHashTable(ht);

 


    

}
