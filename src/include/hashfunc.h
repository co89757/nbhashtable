//
// Created by colin on 12/1/15.
//

#ifndef NBHASHTABLE_HASHFUNC_H
#define NBHASHTABLE_HASHFUNC_H

#include <stdint.h>

typedef uint32_t (*hashCode_t)(uint64_t key);

/**
 * Murmur hash function
 */
static
inline uint32_t murmur32_8b (uint64_t key)
{
    // 'm' and 'r' are mixing constants generated offline.
    // They're not really 'magic', they just happen to work well.

    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    // Initialize the hash to a 'random' value
    uint32_t h = 8;

    uint32_t k1 = (uint32_t)(key >> 32);
    uint32_t k2 = (uint32_t)key;

    k1 *= m;
    k1 ^= k1 >> r;
    k1 *= m;

    k2 *= m;
    k2 ^= k2 >> r;
    k2 *= m;

    // Mix 4 bytes at a time into the hash

    h *= m;
    h ^= k1;
    h *= m;
    h ^= k2;

    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}


/** Hash function for string type */
static
 inline uint32_t murmur32 (const char *key, int len)
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


#endif //NBHASHTABLE_HASHFUNC_H
