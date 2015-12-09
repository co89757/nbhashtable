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


// #define NUM_THREADS 256
#define KEYSLOT 1
#define INIT_VAL 9999
#define TOTAL_ITERS (1<<25UL)
#define SEED time(NULL)
//random number in [lo,hi)
#define RAND(lo,hi) (rand()%((hi)-(lo)) + (lo) )

/** log of the number of keys to put/get in the table for each test */
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

/** function prototypes */
void writeOnlyTest(void* ht, int N, int upperBound, HT_IMPL_TYPE impl, size_t NUM_THREADS);
void readOnlyTest(void* ht, int N, int upperBound, HT_IMPL_TYPE impl, size_t NUM_THREADS);
void readWriteTest(void* ht, int N, int upperBound, HT_IMPL_TYPE impl, size_t NUM_THREADS);



/*** define universal interface for put/get method */
typedef uint64_t (*map_put_t)(void* ht, uint64_t key, uintptr_t value) ;
typedef uintptr_t (*map_get_t)(void* ht, uint64_t key);
typedef void (*map_test_t)(void* ht, int N, int upper, HT_IMPL_TYPE htimpl, size_t nthds);

/** function router tables  */
static 
map_put_t PUT_METHODS[] = {(map_put_t) ht_put_unsafe,(map_put_t)  LockHt_put2,(map_put_t)  LockHt_put};

static 
map_get_t GET_METHODS[] = {(map_get_t) ht_get_unsafe,(map_get_t) LockHt_get2,(map_get_t) LockHt_get};

static 
map_test_t TEST_BENCHMARKS[] = {(map_test_t) writeOnlyTest,(map_test_t) readOnlyTest, (map_test_t) readWriteTest  };

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

/**
 * Fisher-Yates shuffle 
 * @param  A [input array to shuffle]
 * @param  N [array size]
 * @return   [shuffled array]
 */
int* shuffledArray(int* A, int N)
{
    int* outA = calloc(N, sizeof(int));
    //copy first 
    #pragma omp parallel for 
    for (int i = 0; i < N; ++i)
    {
        outA[i] = A[i];
    }
    int t;
    for (int i = 0; i < N-1; ++i)
    {
        int j = RAND(i,N);
        t = outA[i];
        outA[i] = outA[j];
        outA[j] = t ;
    }

    return outA;
}

void writeOnlyTest(void* ht, int N, int upperBound, HT_IMPL_TYPE impl, size_t NUM_THREADS)
{
    map_put_t put = PUT_METHODS[impl];
    int* rdset = randomArray(N, upperBound);

    put(ht,KEYSLOT,INIT_VAL);
    double t0 = omp_get_wtime();
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int tid = omp_get_thread_num();
        Key_t oldVal = tid + 1;
        for (int i = 0; i < TOTAL_ITERS/(NUM_THREADS); ++i) {
            oldVal = put(ht,KEYSLOT, oldVal );
        }

    }
    double t1 = omp_get_wtime();
    //HT,BENCHMARK,NUM-THREADS,TIME
    printf("%s, %s, %zu, %.16g\n", HT_NAMES[impl],BENCHMARKS[0],NUM_THREADS, 1000*(t1-t0) );
    FREE(rdset);
}



void readOnlyTest(void* ht, int N, int upperBound, HT_IMPL_TYPE impl, size_t NUM_THREADS)
{
    map_put_t put = PUT_METHODS[impl];
    map_get_t get = GET_METHODS[impl];

   // prime the table with k-v pairs 
    int* keys = randomArray(N, upperBound);
    for ( int i = 0; i < N; ++i)
    {
        put(ht, keys[i],keys[i]);
    }

    // start profile 
    double t0 = omp_get_wtime();
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < TOTAL_ITERS; ++i)
    {
        /*for (int  j = 0; j < TOTAL_ITERS; ++j)
        {*/
           get(ht, keys[i%N]); 
        // }
    }

    double t1 = omp_get_wtime();
    //HT,BENCHMARK,NUM-THREADS,TIME
    printf("%s, %s, %zu, %.16g\n", HT_NAMES[impl],BENCHMARKS[1],NUM_THREADS, 1000*(t1-t0) );

    FREE(keys);
}

void readWriteTest(void* ht, int N, int upperBound, HT_IMPL_TYPE impl, size_t NUM_THREADS)
{
    map_put_t put = PUT_METHODS[impl];
    map_get_t get = GET_METHODS[impl];

   // prime the table with k-v pairs (half of all keys for reads) 
    int* keys = randomArray(N, upperBound);
    int* shuffledkeys = shuffledArray(keys, N);
    for (int i = 0; i < N; ++i)
    {
        put(ht, keys[i],keys[i]);
    }

    //start test
    double t0 = omp_get_wtime();
    int n_1 = N-1;
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < TOTAL_ITERS; ++i)
    {
        if((i & 0x1) == 0) 
            get(ht, keys[i & n_1]);
        else 
            put(ht, shuffledkeys[i & n_1], INIT_VAL);
               
    }
    double t1 = omp_get_wtime();
    //HT,BENCHMARK,NUM-THREADS,TIME
    printf("%s, %s, %zu, %.16g\n", HT_NAMES[impl],BENCHMARKS[2],NUM_THREADS, 1000*(t1-t0) );

    FREE(keys);
    FREE(shuffledkeys);
}


int main(int argc, char** argv)
{
    size_t nthreads = 4;
    size_t benchmark_id = 0;
    if(argc == 1) {
        fprintf(stderr, "[WARNING] No arguments given! DEFAULT: (NUM_THREADS,BENCHMARK_ID) = (4,0)\n");
    }
    else if (argc == 2){
        nthreads = atoi(argv[1]);   
        fprintf(stderr, "[WARNING] Benchmark ID is not given! Default: write-only (0)\n");
        fprintf(stderr, "[INFO] NUM_THREADS is given as an argument:%zu\n", nthreads);
        assert( (nthreads & (nthreads-1) ) == 0); // must be power of 2 

    }
    else {
        //argc >= 3 
        nthreads = atoi(argv[1]);
        benchmark_id = atoi(argv[2]);
        assert( (nthreads & (nthreads-1) ) == 0); // must be power of 2 
        assert( (benchmark_id >= 0) && (benchmark_id < 3) ); //range check for benchmark id 
        fprintf(stderr, 
            "[INFO] Provided arguments: (NUM_THREADS,BENCHMARK)=(%zu,%s)\n", nthreads, BENCHMARKS[benchmark_id]);
    }

    //testWriteHeavy(LOG_NUM_KEYS, 25000);
    //
    //////////SETUP////////////////
    int logN = LOG_NUM_KEYS;
    Hashtable_t* nbht = ht_newHashTable(logN+1);
    LockHt_Hashtable_t* lockht1 = LockHt_newHashtable(logN+1,NULL );
    LockHt_Hashtable_t* lockht2 = LockHt_newHashtable(logN+1,NULL);

    int N = 1 << LOG_NUM_KEYS;
    HT_IMPL_TYPE ht_types[] = {NON_BLOCKING,COURSE_LOCK,FINE_LOCK};
    //choose benchmark to run 
    map_test_t benchmark = TEST_BENCHMARKS[benchmark_id];

    //START TEST 
    benchmark(nbht, N, 25000, ht_types[0], nthreads);
    benchmark(lockht1, N, 25000, ht_types[1], nthreads);
    benchmark(lockht2, N, 25000, ht_types[2], nthreads);

    //free all tables
    ht_freeHashTable(nbht);
    LockHt_freeHashtable(lockht1);
    LockHt_freeHashtable(lockht2);

    return 0;

}
