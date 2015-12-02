#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>

#include "def.h"
#include "nbhash.h"
#include "collection.h"



#define NUM_THREADS 512
#define N 10
#define M 100
#define VINIT (9999)
#define KEY 1 
#define TESTRUNS 15

static inline
size_t _sum(int lo, int hi){
  size_t s = 0;
  int i;
  for(i=lo;i<=hi;i++)
    s+= i;
  return s;

}



int testValueConservation()
{

    /** ----------------Test Setup ----------------- */
    int bPass = FALSE;
    Hashtable_t * ht = ht_newHashTable(10);
    
    size_t refSum = _sum(1,NUM_THREADS) + VINIT;
    size_t atomicSum = 0;
    size_t* pSum = &atomicSum;
 
    /**  LOGGING SETUP  */
    const char* logname = "debug.log";
    FILE* fp;
    if((fp=fopen(logname,"a+")) == NULL ){
      puts("log file can't open, quitting\n");
      return -1;
    }


    /**  Data collection setup  */
    
    Vector_t* vReadBacks = Vector_new(NUM_THREADS + 4);

    /** ----------------END SETUP- --------------- */



    /**-------------- TEST BODY ------------------ */
    /**
     * All thread repeatedly put the last seen old returned
     * value back to the same entry slot, test for 
     * returned value conservation. Initial put value is thread-ID
     * In the end, the set of eventual readBacks should consists of 
     * all the TIDs and the initial value inside the slot.
     */
   int e = 1;

   /** put initial value in the slot VINIT */
   ht_put(ht, KEY, VINIT,&e );

   if(e != NO_ERROR)
      ERR("Initial put fails");
  



   /** start parallel section */
   #pragma omp parallel num_threads(NUM_THREADS)
   {
      int tid = omp_get_thread_num()+1;
      int err = NO_ERROR;
      int* pErr = &err;


      Val_t oldVal = tid;
      int i=0;
      for(;i<M;i++){
          oldVal = ht_put(ht,KEY,oldVal,pErr);
      }

      ATOMIC_ADD(pSum, oldVal);
      Vector_push(vReadBacks, oldVal);
      fprintf(fp, "LAST VALUE: %u TID:%d\n", oldVal, tid );
   }
   /** read the final value in the slot */
   Val_t finalVal  = ht_get(ht,KEY,&e);
   ATOMIC_ADD(pSum,finalVal);
   Vector_push(vReadBacks, finalVal);


   printf("######## TEST RUN #########\n");


  /** ------------TEST RESULT VALIDATION-------------*/
   int64_t testSum = Vector_sum(vReadBacks);

   if(atomicSum == refSum){
      printf("----------PASSED!----------\n");
      bPass = TRUE;
   }
   else{
      printf("SUM NOT EQUAL:\nExpect: %u \nActual: %u \nMissing Value: %d \n", refSum,atomicSum, (refSum - atomicSum) );

      Vector_sort(vReadBacks);
      Vector_fprintf(vReadBacks, "sorted_readbacks.log");

   }


   /** --------TearDown-------------- */

    ht_print(ht);
    int slots = ht_claimedSlots(ht);
    LOG("Claimed slots: %d\n",slots);
    ht_freeHashTable(ht);
    Vector_free(vReadBacks);
    fclose(fp);

    return bPass;
} 







int main()
{
int i;

for ( i = 0; i < TESTRUNS; ++i)
  if(! testValueConservation() ){
    printf("----TEST FAILURE----\n");
    break;
  }

return 0;

}
