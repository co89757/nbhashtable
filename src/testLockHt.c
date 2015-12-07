//
// Created by colin on 12/2/15.
//
#include <omp.h>
#include <stdio.h>
#include <assert.h>
#include "lockht.h"
#include "nstring.h"

#define NUM_THREADS 512
#define N 10
#define M 30000
#define VINIT (9999)
#define KEY 1
#define TESTRUNS 5


static inline
size_t _sum(int lo, int hi) {
    size_t s = 0;
    int i;
    for (i = lo; i <= hi; i++)
        s += i;
    return s;

}

int lock_testPut()
{
    LockHt_Hashtable_t* ht  =  LockHt_newHashtable(10,NULL);
    LockHt_put(ht,10,100);
    LockHt_Val_t val = LockHt_get(ht,10);
    return val == 100;
}



int lock_testValConservation()
{
    LockHt_Hashtable_t* ht = LockHt_newHashtable(N,NULL);
    int bPass = FALSE;
    size_t expSum = _sum(1,NUM_THREADS) + VINIT;
    size_t actualSum = 0;

    FILE* fp = fopen("lock_debug.log","a+");
    if(!fp)
    {
        puts("file cannot open! quit\n");
        return FALSE;
    }


    /** TEST BODY */
    LockHt_put(ht,KEY,VINIT);

    LockHt_Val_t init_v = LockHt_get(ht,KEY);
    assert(init_v == VINIT);

double startt = omp_get_wtime();
    /** start parallel section */
#pragma omp parallel num_threads(NUM_THREADS)
    {
        int tid = omp_get_thread_num() + 1;


        LockHt_Val_t  oldVal = tid;
        int i = 0;
        for (; i < M; i++) {
//#pragma omp critical
//            {
                oldVal = LockHt_put(ht, KEY, oldVal);
//            }

        }

        ATOMIC_ADD(&actualSum, oldVal);
        //fprintf(fp, "LAST VALUE: %u TID:%d\n", oldVal, tid);
    }

    double endt = omp_get_wtime();

    printf("Time-taken: %.16g [ms]\n ", 1000.0* (endt-startt) );

    LockHt_Val_t finalVal = LockHt_get(ht, KEY);
    ATOMIC_ADD(&actualSum, finalVal);


    /** VERIFY TEST **/
    if (actualSum == expSum) {
        //printf("----------PASSED!----------\n");
        bPass = TRUE;
    }
    else {
        printf("SUM NOT EQUAL:\nExpect: %u \nActual: %u \nMissing Value: %d \n", expSum, actualSum,
               (expSum - actualSum));
    }

    /**   TEARDOWN  */
    LockHt_freeHashtable(ht);
    fclose(fp);
    return bPass;

}





int main()
{
    int i;
    int bPass = TRUE;
    for ( i = 0; i < TESTRUNS; ++i) {
        if (!lock_testValConservation()){
            bPass = FALSE;
            break;
        }
    }
    bPass?puts("ALL-TEST-PASSED!\n") : puts("TEST-FAILED\n");
}