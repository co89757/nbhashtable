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
int ns_compare(const nstring_t* ns1, const nstring_t* ns2);
uint32_t ns_hash(const nstring_t* ns);
nstring_t* ns_dup(const nstring_t* ns);

/** Hash function for string type */
static inline uint32_t murmur32 (const char *key, int len)
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value
	uint32_t h = len;

	// Mix 4 bytes at a time into the hash

	const unsigned char *data = (const unsigned char *)key;

	while(len >= 4)
	{
		uint32_t k = *(uint32_t *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}
	
	// Handle the last few bytes of the input array

	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
	        h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
} 



extern const Type_t TYPE_NSTRING;

#endif