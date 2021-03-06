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


/**
 * get pointer to the key at entry indexed at idx
 */
static inline
volatile Key_t*  getKeyPtr(Hashtable_t *ht, size_t idx)
{
    assert(ht!=NULL && idx < ht->maxsize );
    return &ht->entries[idx].key;
}
static inline
Key_t getKeyContent(Hashtable_t* ht,size_t idx)
{
    Key_t k = ATOMIC_READ(&ht->entries[idx].key);
    return k;
}

/**
 * get pointer to value at entry [idx]
 */
static inline
volatile Val_t*  getValPtr(Hashtable_t *ht, size_t idx)
{
    assert(ht!=NULL && idx < ht->maxsize );
    return &ht->entries[idx].val;
}

static inline 
Val_t getValContent(Hashtable_t* ht, size_t idx)
{
    Val_t v = ATOMIC_READ(&ht->entries[idx].val);
    return v;
}

/**
 * increment the used slots count
 */
static inline
void incSlots(Hashtable_t* ht)
{
    ATOMIC_INC(&ht->_slots);
}

/*static inline
void incLiveKeys(Hashtable_t* ht)
{
   ATOMIC_DEC(&ht->_livekeys);
}*/

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
 * heuristic for reprobe limit
 */
static inline
int reprobe_limit(size_t len)
{
    return REPROBE_LIMIT + (len >> 2 );
}


/**
 * putIfMatch. returns the old value
 */
static
Val_t ht_putIfMatch(Hashtable_t* ht, Key_t key, Val_t putVal, Val_t expVal, int * error)
{
    assert(ht!=NULL);
    assert(key!=NIL);
    assert(error);
    *error = NO_ERROR;
    uint32_t fullhash = murmur32_8b(key);
    size_t len = ht->maxsize;
    size_t idx = fullhash & (len-1);
    size_t reprobes_cnt = 0;

    volatile  Key_t * pK = NULL;
    volatile  Val_t * pV = NULL;
    pK = getKeyPtr(ht, idx);
    pV = getValPtr(ht,idx);
    //start loop to claim key slot
    while (TRUE) {
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
                LOG("claimed key-slot:%u",key);
                ht->hashes[idx] = fullhash; //cache fullhash
                break;
            }

        }

        //if key already exists
        //compare key
       
        if(isKeyEqual(*pK, key))
            break; //got the key!

        //else key not equal, we must reprobe and check reprobe limit
        if(++reprobes_cnt >= reprobe_limit(len) || getSlots(ht) > (size_t)(0.8 * ht->maxsize )  )
        {
            *error = ERROR_FULLTABLE;
            ERR("reprobe limit is reached when putting (K,V)=(%u,%u)", key,putVal );
            return NIL;
        }
        //reprobe
        idx = (1+idx) & (len-1);

    }

    //we got the key! update the value now
    if(putVal == *pV)  
        return putVal; //fast cut-out for no change
 
    
    Val_t V = getValContent(ht,idx); // variable to store the current value
    while (TRUE){
        
        if( expVal != CAS_EXPECT_WHATEVER  && //do we care about expected value at all?
                V != expVal &&
                (expVal != CAS_EXPECT_EXIST || V==TOMBSTONE || V == NIL) &&
                !(V == NIL && expVal == TOMBSTONE)
                )
        {
            *error = ERROR_PUTFAIL;
            ERR("put value failed. expVal=%d , V=%d" , expVal,*pV);
            return V;   //do not update.
        }


        /** CAS-val, on CAS failure, V will be automatically updated to current value */
        if(ATOMIC_CAS( pV , &V, putVal, FALSE  )){  
            return V;
        }

    }

} //END OF PUT_IF_MATCH


/**
 * get implementation
 */
static
Val_t ht_getImpl(Hashtable_t* ht, Key_t key, uint32_t fullhash, int* error)
{
    assert(ht);
    assert(error);
    *error = NO_ERROR;
    size_t len = ht->maxsize;
    size_t idx = fullhash & (len-1);
    int reprobe_cnt = 0;
    while (TRUE)
    {
        volatile  Key_t * pK = getKeyPtr(ht,idx);
        volatile  Val_t * pV = getValPtr(ht,idx);
        //key not exist, a miss
        if(*pK == NIL){
            *error = ERROR_NULLKEY;
            ERR("Error in get_impl(), K is NULL");
            return NIL;
        }
        //we have a key match
        if(isKeyEqual(*pK, key)){
            return (*pV == TOMBSTONE)? NIL: (*pV);
        }
        //else, test reprobe_limit condition and reprobe
        if(++reprobe_cnt > reprobe_limit(len) ){
            *error = ERROR_FULLTABLE;
            ERR("reprobe limit is reached when doing lookup");
            return NIL;
        }

        //reprobe
        idx = (idx+1) & (len-1);


    }

}



/**------------------------------------------**/
/**----------- Public API ------------------****/
/** ------------------------------------------- */
Hashtable_t* ht_newHashTable(size_t size_log)
{

    assert(size_log > MIN_SIZE_LOG);

    Hashtable_t* poHt = (Hashtable_t*) calloc(1, sizeof(Hashtable_t) );
    if(poHt == NULL){
        ERR("Allocation failure" );
        return NULL;
    }

    size_t nSize = 1 << size_log ;
    //poHt->_livekeys = 0;
    poHt->_slots = 0;
    poHt->maxsize = nSize;
    poHt->entries = (Entry_t*) calloc(nSize, sizeof(Entry_t)  );
    poHt->hashes = (uint32_t *) calloc(nSize, sizeof(uint32_t) );
    /** key type by default is plain uint64_t */
    poHt->key_type = NULL;
    if(!poHt->entries || !poHt->hashes){
        ERR( "Alocation failure");
        return NULL;
    }
 

    return poHt;

}

void ht_freeHashTable(Hashtable_t* ht)
{
    FREE(ht->entries);
    FREE(ht->hashes);
    FREE(ht);
}


// --- get -----------------------------------------------------------------
/** Returns the value to which the specified key is mapped, or {@code null}
 *  if this map contains no mapping for the key.
 *  <p>More formally, if this map contains a mapping from a key {@code k} to
 *  a value {@code v} such that {@code key.equals(k)}, then this method
 *  returns {@code v}; otherwise it returns {@code null}.  (There can be at
 *  most one such mapping.)
 * @throws NullPointerException if the specified key is null */

Val_t ht_get(Hashtable_t* self, Key_t key, int * error)
{

    uint32_t fullhash = murmur32_8b(key);
    Val_t V = ht_getImpl(self,key,fullhash,error);
    return V;
}


Val_t ht_put(Hashtable_t* self, Key_t key, Val_t value, int* error)
{
    return ht_putIfMatch(self,key,value,CAS_EXPECT_WHATEVER, error);
}


Val_t ht_remove(Hashtable_t* self, Key_t key, int* error)
{
    return ht_putIfMatch(self,key,TOMBSTONE,CAS_EXPECT_WHATEVER,error);
}


void ht_print(Hashtable_t* self)
{
    if(!self){
        ERR("null table pointer");
        return;
    }

    Entry_t * ents = self->entries;
    if(!ents) return;
    int len = self->maxsize;
    int i;
    printf("..........Printout K-V pairs............\n");

    int cnt = 0;
    for(i=0;i<len;i++){
    
        if(ents[i].key!=NIL){
            cnt ++ ;
            printf( "{%u:%u}\n",ents[i].key,ents[i].val);
            if(cnt % 20 == 0) 
                printf(" .....\n");
        }
         
    }

}

int ht_isEmpty(Hashtable_t* self)
{
    size_t slots = getSlots(self);
    return slots != 0 ? FALSE:TRUE;
}

int ht_claimedSlots(Hashtable_t* ht)
{
    return ATOMIC_READ(&ht->_slots);
}

int ht_hasKey(Hashtable_t* ht, Key_t key)
{
    int e = 1;
    int* err = &e;
    Val_t v = ht_get(ht,key,err);
    return (*err == NO_ERROR && v != NIL )?TRUE:FALSE;
}