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


#define SEED time(NULL)
//random number in [lo,hi)
#define RAND(lo,hi) (rand()%((hi)-(lo)) + (lo) )
#define LOG_NUM_KEYS 12

typedef enum {
    NON_BLOCKING,
    FINE_LOCK,
    COURSE_LOCK
} HtImplType;


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

void printHash(hashCode_t hash, size_t loglen, size_t lo, size_t hi)
{
    size_t j;
    size_t N = 1 << loglen;
    FILE* fp = fopen("index.csv","a+");
    fprintf(fp, "key,index\n");
    for (j = lo; j < hi+1; ++j) {
        uint32_t idx = hash(j) & (N-1);
        fprintf(fp,"%u,%u\n",j,idx);
    }
    fclose(fp);
}

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


void nb_writeHeavy(Hashtable_t* ht, int N, int upperBound)
{
    int* rdset = randomArray(N, upperBound);
    int keys_per_thread = N/(NUM_THREADS);
    double t0 = omp_get_wtime();
#pragma omp parallel num_threads(NUM_THREADS)
    {
        int tid = omp_get_thread_num();
        int i;
        for ( i = 0; i < keys_per_thread; ++i) {
            Key_t key = rdset[tid+i];
            int e = NO_ERROR;
            ht_put(ht, key, tid+1,&e );
        }


    }
    double t1 = omp_get_wtime();
    //HT,BENCHMARK,NUM-THREADS,TIME

    printf("%s, %s, %d, %.16g\n", HT_NAMES[0],BENCHMARKS[0],NUM_THREADS, 1000*(t1-t0) );
    //ht_freeHashTable(ht);
    FREE(rdset);
}

void lk_writeHeavy_course(LockHt_Hashtable_t* ht, int N, int upperBound)
{
    int* rdset = randomArray(N+1, upperBound);
    int keys_per_thread = N/(NUM_THREADS);
    double t0 = omp_get_wtime();

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int tid = omp_get_thread_num();
        int i;
#pragma omp critical
        {
            for (i = 0; i < keys_per_thread; ++i) {
                Key_t key = rdset[tid + i];

                LockHt_put2(ht, key, tid + 1);

            }
        }

    }
    double t1 = omp_get_wtime();
    //HT,BENCHMARK,NUM-THREADS,TIME

    printf("%s, %s, %d, %.16g\n", HT_NAMES[1],BENCHMARKS[0],NUM_THREADS, 1000*(t1-t0) );

    //LockHt_freeHashtable(ht);
    FREE(rdset);
}

void lk_writeHeavy_fine(LockHt_Hashtable_t* ht, int N, int upperBound)
{
    int* rdset = randomArray(N, upperBound);
    int keys_per_thread = N/(NUM_THREADS);
    double t0 = omp_get_wtime();

#pragma omp parallel num_threads(NUM_THREADS)
    {
        int tid = omp_get_thread_num();
        int i;
        for (i = 0; i < keys_per_thread; ++i) {
            Key_t key = rdset[tid+i];
            LockHt_put(ht, key, tid+1 );
        }


    }
    double t1 = omp_get_wtime();
    //HT,BENCHMARK,NUM-THREADS,TIME
    printf("%s, %s, %d, %.16g\n", HT_NAMES[2],BENCHMARKS[0],NUM_THREADS, 1000*(t1-t0) );
    //LockHt_freeHashtable(ht);
    FREE(rdset);
}
/**
 * logN: log of N(#keys to put)
 */
void testWriteHeavy(int logN, int upperBound)
{

    Hashtable_t* nbht = ht_newHashTable(logN+1);
    LockHt_Hashtable_t* lockht1 = LockHt_newHashtable(logN+1,NULL );
    LockHt_Hashtable_t* lockht2 = LockHt_newHashtable(logN+1,NULL);
    int num_keys = 1 << logN;
    assert(num_keys % NUM_THREADS == 0 );

    //test loop 
    nb_writeHeavy(nbht,num_keys,upperBound);
    lk_writeHeavy_course(lockht1,num_keys,upperBound);
    lk_writeHeavy_fine(lockht2,num_keys,upperBound);

    //free all tables
    ht_freeHashTable(nbht);
    LockHt_freeHashtable(lockht1);
    LockHt_freeHashtable(lockht2);
}


int main(int argc, char** argv)
{
    testWriteHeavy(LOG_NUM_KEYS, 25000);

}
