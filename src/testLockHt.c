//
// Created by colin on 12/2/15.
//
#include <omp.h>
#include "lockht.h"
#include "nstring.h"


int testPut()
{
    LockHt_Hashtable_t* ht  =  LockHt_newHashtable(10,NULL);
    LockHt_put(ht,10,100);
    LockHt_Val_t val = LockHt_get(ht,10);
    return val == 100;
}


int main()
{
    if(testPut())
        printf("PASS!\n");
    else
        puts("FAIL!\n");
}