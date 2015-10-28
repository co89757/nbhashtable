#ifndef _DEF_H_
#define _DEF_H_ 

#define FALSE 0
#define TRUE 1 

/**  Atomic operations */

#define MEMORDER __ATOMIC_SEQ_CST 
#define FAIL_ORDER __ATOMIC_RELAXED
#define ATOMIC_READ(x) __atomic_load_n((x), MEMORDER) 
#define ATOMIC_WRITE(x,val) __atomic_store_n((x),(val), MEMORDER )
#define ATOMIC_CAS(ptr,exp,desired,bweak) __atomic_compare_exchange_n((ptr),(exp),(desired),(bweak),MEMORDER,FAIL_ORDER)
#define ATOMIC_INC(ptr) __atomic_fetch_add((ptr),1,MEMORDER)


/** Errors */
#define ERROR_OK 0
#define ERROR_FULLTABLE -1
#define ERROR_INVALID_ARGUMENT -2
#define ERROR_NOT_ENOUGH_MEMORY -3






#endif