//
// Created by colin on 12/2/15.
//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "lockht.h"
#include "nbhash.h"
#include "hashfunc.h"
#include <time.h>


#define NUM_THREADS 256
#define KEYSLOT 1
#define INIT_VAL 9999
#define INNER_ITER 3000
#define SEED time(NULL)
//random number in [lo,hi)
#define RAND(lo,hi) (rand()%((hi)-(lo)) + (lo) )
#define LOG_NUM_KEYS 12

typedef enum {
    NON_BLOCKING,  
    COURSE_LOCK,
    FINE_LOCK
} HT_IMPL_TYPE;


static const char* HT_NAMES[] = {
        "Lock-Free",
        "Course-Grain-Lock",
        "Fine-Grain-Lock"
};

static const char* BENCHMARKS[] = {
  "Write-Heavy",
  "Read-Heavy",
  "Write-Read"
};


/*** define universal interface for put/get method */
typedef uint64_t (*map_put_t)(void* ht, uint64_t key, uintptr_t value) ;
typedef uintptr_t (*map_get_t)(void* ht, uint64_t key);

static 
map_put_t PUT_METHODS[] = {(map_put_t) ht_put_unsafe,(map_put_t)  LockHt_put2,(map_put_t)  LockHt_put};

static 
map_get_t GET_METHODS[] = {(map_get_t) ht_get_unsafe,(map_get_t) LockHt_get2,(map_get_t) LockHt_get};

/**
 * generate a set of random numbers within the bound [upper-N, upper)
 */
int* randomArray(int N, int upper)
{
    assert(upper-N > 0);
    srand(SEED);
    int* set = calloc(N, sizeof(int));
    int i = 0;
    int d;
    for ( d = upper-N; d < upper; ++d, i++) {
            int x = RAND(1,d);
            set[i]  = x;
    }

    return set;

}

void writeOnlyTest(void* ht, int N, int upperBound, HT_IMPL_TYPE impl)
{
    map_put_t put = PUT_METHODS[impl];
    int* rdset = randomArray(N, upperBound);
    int keys_per_thread = N/(NUM_THREADS);

    put(ht,KEYSLOT,INIT_VAL);
    double t0 = omp_get_wtime();
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int tid = omp_get_thread_num();
           int i;
//        for ( i = 0; i < keys_per_thread; ++i) {
//            Key_t key = rdset[tid+i];
//            put(ht,key,tid+1);
//        }

        Key_t oldVal = tid + 1;
        for ( i = 0; i < INNER_ITER; ++i) {
            oldVal = put(ht,KEYSLOT, oldVal );
        }


    }
    double t1 = omp_get_wtime();
    //HT,BENCHMARK,NUM-THREADS,TIME
    printf("%s, %s, %d, %.16g\n", HT_NAMES[impl],BENCHMARKS[0],NUM_THREADS, 1000*(t1-t0) );
    FREE(rdset);
}



void readOnlyTest(void* ht, int N, int upperBound, HT_IMPL_TYPE impl)
{
    map_put_t put = PUT_METHODS[impl];
    map_get_t get = GET_METHODS[impl];

   // int i,j;
    int* keys = randomArray(N, upperBound);
    for ( int i = 0; i < N; ++i)
    {
        put(ht, keys[i],keys[i]);
    }

    // start profile 
    double t0 = omp_get_wtime();
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < N*INNER_ITER; ++i)
    {
        /*for (int  j = 0; j < INNER_ITER; ++j)
        {*/
           get(ht, keys[i%N]); 
        // }
    }

    double t1 = omp_get_wtime();
    //HT,BENCHMARK,NUM-THREADS,TIME
    printf("%s, %s, %d, %.16g\n", HT_NAMES[impl],BENCHMARKS[1],NUM_THREADS, 1000*(t1-t0) );

    FREE(keys);
}



int main(int argc, char** argv)
{
    //testWriteHeavy(LOG_NUM_KEYS, 25000);
    //
    //////////SETUP////////////////
    int logN = LOG_NUM_KEYS;
    Hashtable_t* nbht = ht_newHashTable(logN+1);
    LockHt_Hashtable_t* lockht1 = LockHt_newHashtable(logN+1,NULL );
    LockHt_Hashtable_t* lockht2 = LockHt_newHashtable(logN+1,NULL);

    int N = 1 << LOG_NUM_KEYS;
    HT_IMPL_TYPE types[] = {NON_BLOCKING,COURSE_LOCK,FINE_LOCK};

    // writeOnlyTest(nbht, N, 25000, types[0]);
    // writeOnlyTest(lockht1, N, 25000, types[1]);
    // writeOnlyTest(lockht2, N, 25000, types[2]);

    readOnlyTest(nbht,N,25000, types[0]);
    readOnlyTest(lockht1,N,25000, types[1]);
    readOnlyTest(lockht2,N,25000, types[2]);


    //free all tables
    ht_freeHashTable(nbht);
    LockHt_freeHashtable(lockht1);
    LockHt_freeHashtable(lockht2);

    return 0;

}
