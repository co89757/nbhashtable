#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "def.h"
#include "nbhash.h"

#define MIN_SIZE_LOG (3)
#define TOMBSTONE (0)
#define CAS_EXPECT_WHATEVER (-1)
#define CAS_EXPECT_EXIST (-2)
#define NIL (0)
/******************************************************/
/********* Private Methods ****************************/
/******************************************************/

/**
 * Murmur hash function
 */
static inline uint32_t murmur32_8b (uint64_t key)
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

static inline
Key_t* key(Hashtable_t* ht, size_t idx)
{
    assert(ht!=NULL && idx < ht->maxsize );
    return &ht->entries[idx].key;
}
static inline
Val_t* val(Hashtable_t* ht, size_t idx)
{
    assert(ht!=NULL && idx < ht->maxsize );
    return &ht->entries[idx].val;
}

static inline
void incSlots(Hashtable_t* ht)
{
    ATOMIC_INC(&ht->_slots);
}

static inline
void incLiveKeys(Hashtable_t* ht)
{
   ATOMIC_DEC(&ht->_livekeys);
}

static inline
size_t getSlots(Hashtable_t* ht)
{
    size_t ret = ATOMIC_READ(&ht->_slots);
    return ret;
}

static inline
int isKeyEqual(Key_t k, Key_t key){
    return k == key ? TRUE:FALSE;
}


/**
 * putIfMatch. returns the old value
 */
static
Val_t ht_putIfMatch(Hashtable_t* ht, Key_t key, Val_t putVal, Val_t expVal, int * error)
{
    assert(ht!=NULL);
    assert(key!=NIL);
    *error = NO_ERROR;
    uint32_t fullhash = murmur32_8b(key);
    size_t len = ht->maxsize;
    size_t idx = fullhash & (len-1);
    size_t reprobes_cnt = 0;
    Key_t * pK = NULL;
    Val_t * pV = NULL;

    //start loop to claim key slot
    while (TRUE) {
          pK = key(ht, idx);
          pV = val(ht,idx);

        //if slot is free?
        if(*pK == NIL)
        {
            //quick cut-out
            if(putVal == TOMBSTONE) return putVal;
            Key_t temp = NIL;
            Key_t * pExpKey = &temp;
            //claim the key slot
            if(ATOMIC_CAS(pK,pExpKey,key,FALSE) )
            {
                //occupied a new key, increment slots
                incSlots(ht);
                ht->hashes[idx] = fullhash; //cache fullhash
                break;
            }

            //CAS failed, repeat
            pK = key(ht,idx);
            assert(pK);
        }

        //if key already exists
        //compare key
        if(isKeyEqual(*pK, key))
            break; //got the key!
        //else key not equal, we must reprobe and check reprobe limit
        if(++reprobes_cnt >= REPROBE_LIMIT || getSlots(ht) > (size_t)(0.8 * ht->maxsize )  )
        {
            *error = ERROR_FULLTABLE;
            ERR(errFullTable,"reprobe limit is reached");
            return NIL;
        }
        //reprobe
        idx = (1+idx) & (len-1);

    }

    //we got the key! update the value now
    if(putVal == *pV) return putVal; //fast cut-out for no change

    while (TRUE){
        if( expVal != CAS_EXPECT_WHATEVER  &&
                *pV != expVal &&
                (expVal != CAS_EXPECT_EXIST || *pV==TOMBSTONE || *pV == NIL) &&
                !(*pV == NIL && expVal == TOMBSTONE)
                )
        {
            *error = ERROR_PUTFAIL;
            return *pV;   //do not update.
        }


        //update the value now
        if(ATOMIC_CAS( val(ht), pV, putVal, FALSE  )){

            return *pV;
        }
        //else, CAS failed, get the new value
        pV = val(ht, idx);

    }



}




/***********************************************
/****** Public API **************************/

Hashtable_t* ht_newHashTable(size_t size_log)
{

    assert(size_log > MIN_SIZE_LOG);

    Hashtable_t* poHt = (Hashtable_t*) calloc(1, sizeof(Hashtable_t) );
    if(poHt == NULL){
        ERR(errAllocFail,"Allocation failure in %s:%d", __FILE__,__LINE__ );
        return NULL;
    }

    size_t nSize = 1 << size_log ;
    poHt->_livekeys = 0;
    poHt->_slots = 0;
    poHt->maxsize = nSize;
    poHt->entries = (Entry_t*) calloc(nSize, sizeof(Entry_t)  );
    poHt->hashes = (uint32_t *) calloc(nSize, sizeof(uint32_t) );

    if(!poHt->entries || !poHt->hashes){
        ERR(errAllocFail,"Alocation failure in %s:%d",__FILE__,__LINE__);
        return NULL;
    }

    poHt->get = ht_get;
    poHt->put = ht_put;
    poHt->remove = ht_remove;


}