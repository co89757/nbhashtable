#ifndef _HASHTABLE_H
#define _HASHTABLE_H 

#include <stdint.h>
#include "def.h"
#include "datatype.h"
/** can use uintptr_t */
typedef uint64_t Key_t ;
typedef uintptr_t Val_t ;

struct Counter
{
	int _count;
	void (*dec)(struct Counter* self);
	void (*inc)(struct Counter* self);
	void (*reset)(struct Counter* self);
	int (*get)(struct Counter* self);

};

typedef struct Counter Counter_t ; 


typedef struct 
{
	volatile Key_t key;
	volatile Val_t val;
	
} Entry_t;

 
typedef uint32_t (*hashCode_t)(uint64_t key);


struct Hashtable{
	Entry_t * entries;
	//int _livekeys;
	size_t _slots;
	size_t maxsize;

	uint32_t* hashes;

	/** key type meta info */
	Type_t* key_type;
 

};

typedef struct Hashtable Hashtable_t ;

#define REPROBE_LIMIT (10)

/*-------------------------------------------*/
/** Public methods function declarations */

/**
 * create a new hashtable and init to zeros 
 * @param  sizeLog [log of the array size]
 * @return         [pointer to the HT]
 */
Hashtable_t* ht_newHashTable(size_t sizeLog);
void ht_setKeyType(Hashtable_t* ht, Type_t* pType);

/** free table */
void ht_freeHashTable(Hashtable_t* ht);
/** get value by key, return NIL if the key is not mapped or deleted */
Val_t ht_get(Hashtable_t* self, Key_t key, int * error);
/** put key-value pair ,return old value */
Val_t ht_put(Hashtable_t* self, Key_t key, Val_t value, int* error);
/** remove key-value pair, return old value */
Val_t ht_remove(Hashtable_t* self, Key_t key, int* error);

void ht_print(Hashtable_t* self);
int ht_claimedSlots(Hashtable_t* ht);
int ht_isEmpty(Hashtable_t* self);



#endif
