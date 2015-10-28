#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>
#include "def.h"
#define NUM_THREADS 8
int main(){
 uint64_t count = 0;
#pragma omp parallel num_threads(NUM_THREADS)
 {
 int tid = omp_get_thread_num();
 printf("TID%d touching, counter before = %d\n",tid,count);
// ATOMIC_INC(&count);
 count = count + 1;
 printf("TID%d counter incremented: %d\n\n",tid,count);
 }

 

 printf("count=%d\n",count);
    

}
