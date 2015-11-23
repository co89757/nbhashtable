#include <stdlib.h>
#include <stdio.h>
#include "collection.h"

/**
 * Implementation for vector 
 */

static 
void _resize(Vector_t* self)
{
	NULL_CHECK(self);
	uint64_t newCap = self->M << 1; 
	int64_t* newArray = (int64_t*) calloc(newCap, sizeof(int64_t));
	int64_t* pTemp = self->elems;
	//copy
	int i;
	for(i=0;i<self->size ; ++i){
		newArray[i] = pTemp[i];
	}
	FREE(pTemp);
	self->elems = newArray;
	self->M = newCap;
}


Vector_t* Vector_new(uint64_t capacity)
{
	RANGE_CHECK(capacity, MIN_CAP, UINT_MAX); 
	Vector_t* pV = calloc(1, sizeof(Vector_t));
	pV->M = capacity;
	pV->size = 0;
	pV->elems = calloc(capacity, sizeof(int64_t)); 
	return pV;

}
int64_t Vector_at(Vector_t* self, int idx)
{
	RANGE_CHECK(idx, 0, self->size - 1 );
	return self->elems[idx];

}
void Vector_push(Vector_t* self, int64_t value )
{
	NULL_CHECK(self);
	/** check size */
	if(self->size == self->M - 3)
		_resize(self);
	self->elems[(self->size)++] = value ;

}
int64_t Vector_sum(Vector_t* self)
{
	int64_t sum = 0;
	int i=0;
	int64_t* a = self->elems;
	for(;i<self->size;i++){
		sum += a[i];
	}

	return sum;
}
uint64_t Vector_size(Vector_t* self)
{
	NULL_CHECK(self);
	return self->size;
}
void Vector_free(Vector_t* self)
{
	FREE(self->elems);
	FREE(self);
}
void Vector_sort(Vector_t* self)
{
	int N = self->size;
	if(N<2) return; 
	int i,j,t;
	int64_t* a = self->elems;
	for(i=1;i<N;++i)
		for(j=i; j > 0 && a[j] < a[j-1]; --j){
			t = a[j];
			a[j] = a[j-1];
			a[j-1] = t;
		}

}
int Vector_find(Vector_t* self, int value)
{
	int N = self->size;
	int64_t* a = self->elems;
	int i;
	for(i=0;i<N;i++)
		if(a[i] == value) 
			return i;

	return -1;
}
void Vector_print(Vector_t* self)
{
	int i;
	
	int64_t* a = self->elems;
	for(i=0; i< self->size; i++){
		printf("%d ", a[i] );
		if( (i+1) % 10 == 0 )
			printf("\n   ......   \n");

	}
}

void Vector_fprintf(Vector_t* self, const char* fname)
{
	int i;
	int64_t* a = self->elems;
	FILE* fp = fopen(fname, "a+");
	if(!fp) 
		ERR("FileIO Error: %s cannot open",fname);
	for (  i = 0; i < self->size; ++i)
	{
		 fprintf(fp, "%ld\n", a[i]);
	}

	fclose(fp);
}

inline
int Vector_empty(Vector_t*self)
{
	return self->size == 0;
}
