#ifndef NSTRING_H_
#define NSTRING_H_ 


#include <stdint.h>
#include "datatype.h"

/**
 * Variable-length string type
 */
typedef struct nstring
{
	uint32_t len;
	char data[];
} nstring_t;

/** TYPE_NSTRING API */
nstring_t* ns_new(uint32_t len);
nstring_t* ns_init(const char* str);
int ns_compare(const nstring_t* ns1, const nstring_t* ns2);
uint32_t ns_hash(const nstring_t* ns);
nstring_t* ns_dup(const nstring_t* ns);




extern const Type_t TYPE_NSTRING;

#endif