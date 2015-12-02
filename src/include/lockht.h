//
// Created by colin on 12/1/15.
// a lock-based chaining hashtable implementation
//

#ifndef NBHASHTABLE_LOCKHT_H
#define NBHASHTABLE_LOCKHT_H

#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include "def.h"
#include "datatype.h"
/** -------------- Table Entry Type definition -----------*/

typedef uint64_t LockHt_Key_t;
typedef uintptr_t LockHt_Val_t;

#define LOCKHT_NIL 0

typedef struct LockHt_Entry{
    LockHt_Key_t key;
    LockHt_Val_t val;
    struct LockHt_Entry* next;

} LockHt_Entry_t;

/** ENTRY API ***/
LockHt_Entry_t* LockHt_newEntry(LockHt_Key_t k, LockHt_Val_t v, LockHt_Entry_t* nxt);
void LockHt_freeEntry(LockHt_Entry_t* ent);

/**
 * -------------- Lock-based Chaining Hashtable ----------------*/

typedef struct LockHt_Hashtable{
    size_t num_entries;
    LockHt_Entry_t ** entries;
    Type_t* key_type;
    omp_lock_t* locks;

} LockHt_Hashtable_t;

/***** LockHT API ****/
LockHt_Hashtable_t* LockHt_newHashtable(size_t LogN, Type_t* Ktype);
void LockHt_freeHashtable(LockHt_Hashtable_t* ht);

void LockHt_put(LockHt_Hashtable_t* ht, LockHt_Key_t k, LockHt_Val_t v);

LockHt_Val_t LockHt_get(LockHt_Hashtable_t* ht, LockHt_Key_t k);

void LockHt_remove(LockHt_Hashtable_t* ht, LockHt_Key_t k);

int LockHt_hasKey(LockHt_Hashtable_t* ht, LockHt_Key_t k);










#endif //NBHASHTABLE_LOCKHT_H
