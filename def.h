#ifndef _DEF_H_
#define _DEF_H_ 

#define FALSE 0
#define TRUE 1 

#define LIKELY(x) __builtin_expect(!!(x),1) 
#define UNLIKELY(x) __builtin_expect(!!(x),0)



/**  Atomic operations with implicit fences*/

#define MEMORDER __ATOMIC_SEQ_CST 
#define FAIL_ORDER __ATOMIC_RELAXED
#define ATOMIC_READ(addr) __atomic_load_n((addr), MEMORDER) 
#define ATOMIC_WRITE(addr,val) __atomic_store_n((addr),(val), MEMORDER )
#define ATOMIC_CAS(ptr,exp,desired,bweak) __atomic_compare_exchange_n((ptr),(exp),(desired),(bweak),MEMORDER,FAIL_ORDER)
#define ATOMIC_INC(ptr) __atomic_fetch_add((ptr),1,MEMORDER)
#define ATOMIC_ADD(ptr, addval) __atomic_fetch_add((ptr), (addval) , MEMORDER )
#define ATOMIC_DEC(ptr) __atomic_fetch_sub((ptr),1,MEMORDER)

/** Errors */
#define NO_ERROR 0
#define ERROR_FULLTABLE (-1)
#define ERROR_INVALID_ARGUMENT (-2)
#define ERROR_NOT_ENOUGH_MEMORY (-3)
#define ERROR_PUTFAIL (-4)
#define ERROR_NULLKEY (-5)



#ifdef DEBUG

	#define ERR(fmt, ...)   fprintf(stderr, "[ERROR]:%s:%d " "[MESSAGE]: " fmt "\n", __FILE__,__LINE__, \
	##__VA_ARGS__)

	#define LOG(fmt, ...)   fprintf(stderr, "[DEBUG]: "  fmt "\n", \
	##__VA_ARGS__)

#else
	#define ERR(fmt,...) 
    #define LOG(fmt,...)
#endif

#define FREE(p) do{free((p)); p = NULL;  }while(0)


#endif
