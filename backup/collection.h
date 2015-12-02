#ifndef COLLECTION_H_
#define COLLECTION_H_

/**
 * Common data structure implementation such as vector and set 
 */

#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include "def.h"

#define RANGE_CHECK(idx,lo,hi) assert(!((idx) < (lo) || (idx) > (hi) ))
#define NULL_CHECK(p) assert((p)) 
#define MIN_CAP (16)



typedef struct 
{
	uint64_t M; //capacity 
	uint64_t size ; 
	int64_t * elems ; 
} Vector_t ;

/** API for vector */
Vector_t* Vector_new(uint64_t capacity);
int64_t Vector_at(Vector_t* self, int idx);
void Vector_push(Vector_t* self, int64_t value );
int64_t Vector_sum(Vector_t* self);
uint64_t Vector_size(Vector_t* self);
void Vector_free(Vector_t* self);
void Vector_sort(Vector_t* self);
int Vector_find(Vector_t* self, int val);
void Vector_print(Vector_t* self);
void Vector_fprintf(Vector_t* self, const char* fname);
int Vector_empty(Vector_t*self);

#endif