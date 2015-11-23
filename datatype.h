#ifndef DATATYPE__H__
#define DATATYPE__H__
#include <stdint.h>

typedef int (*compare_fun_t)(void* , void* );
typedef uint32_t (*hash_fun_t)(void* k);
typedef void* (*clone_fun_t)(void* );

/**
 * struct to represent the type information
 */
typedef struct 
{
  	compare_fun_t compare;
  	hash_fun_t hash;
  	clone_fun_t clone;
	
} Type_t;

#endif