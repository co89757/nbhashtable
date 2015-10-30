#ifndef _HASHTABLE_H
#define _HASHTABLE_H 

#include <stdint.h>
#include "def.h"

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

	uint32_t * hashes;

	/** methods */
	Val_t (*get)(struct Hashtable* ht, Key_t key, int * error);
	Val_t (*put)(struct Hashtable* ht, Key_t key, Val_t value, int *error);
	Val_t (*remove)( struct Hashtable* ht , Key_t key, int* error);


};

typedef struct Hashtable Hashtable_t ;

#define REPROBE_LIMIT (10)


/** Public methods function declarations */
Hashtable_t* ht_newHashTable(size_t sizeLog);
void ht_freeHashTable(Hashtable_t* ht);

Val_t ht_get(Hashtable_t* self, Key_t key, int * error);
Val_t ht_put(Hashtable_t* self, Key_t key, Val_t value, int* error);
Val_t ht_remove(Hashtable_t* self, Key_t key, int* error);
void ht_print(Hashtable_t* self);

int ht_isEmpty(Hashtable_t* self);



#endif