//
// Created by colin on 12/1/15.
//
#include <omp.h>
#include <stdlib.h>
#include "lockht.h"
#include "hashfunc.h"

#define ACQUIRE_LOCK(plk) do{ omp_set_lock((plk)); LOG("acquired lock!");}while(0)
#define RELEASE_LOCK(plk) do{ omp_unset_lock((plk));LOG("released lock!"); }while(0)


/** ENTRY API ***/
LockHt_Entry_t *LockHt_newEntry(LockHt_Key_t k, LockHt_Val_t v, LockHt_Entry_t *nxt) {
    LockHt_Entry_t *pEnt = (LockHt_Entry_t *) malloc(sizeof(LockHt_Entry_t));
    pEnt->key = k;
    pEnt->val = v;
    pEnt->next = nxt;

    return pEnt;
}

//!!!BUGGY
//FIXME: need to be recursive free 
void LockHt_freeEntry(LockHt_Entry_t *ent) {
    FREE(ent);
}


/*** HashTable API *****/

static inline
int keyEqual(Type_t *type, LockHt_Key_t key1, LockHt_Key_t key2) {
    if (type == NULL)
        return key1 == key2;
    return type->compare(GET_PTR(key1), GET_PTR(key2)) == 0;
}

static inline
uint32_t getHash(Type_t *type, LockHt_Key_t key) {
    if (!type)
        return murmur32_8b(key);
    return type->hash(GET_PTR(key));
}

LockHt_Hashtable_t *LockHt_newHashtable(size_t LogN, Type_t *Ktype) {
    LockHt_Hashtable_t *pHt = (LockHt_Hashtable_t *) malloc(sizeof(LockHt_Hashtable_t));
    size_t nEnts = 1 << LogN;
    pHt->num_entries = nEnts;
    pHt->key_type = Ktype;
    pHt->entries = (LockHt_Entry_t **) calloc(nEnts, sizeof(LockHt_Entry_t *));
    pHt->locks = (omp_lock_t *) malloc(nEnts * sizeof(omp_lock_t));
    int i;
    //init locks for each entry head
    for (i = 0; i < nEnts; ++i) {
        omp_init_lock(&pHt->locks[i]);
    }

    return pHt;

}

void LockHt_freeHashtable(LockHt_Hashtable_t *ht) {
    size_t nEnts = ht->num_entries;
    //free entries
    int i;
    for (i = 0; i < nEnts; ++i) {
        LockHt_Entry_t *curEnt = ht->entries[i];
        while (curEnt) {
            LockHt_Entry_t *temp_curEnt = curEnt;
            //LockHt_Entry_t *nextEnt = curEnt->next;
            curEnt = curEnt->next;
            LockHt_freeEntry(temp_curEnt);
            
        }
    }
    FREE(ht->entries);

    //free locks
    for (i = 0; i < nEnts; ++i) {
        omp_destroy_lock(&ht->locks[i]);
    }
    FREE(ht->locks);
}

//put k-v pair and return old value (NIL or old value)
LockHt_Val_t LockHt_put(LockHt_Hashtable_t *ht, LockHt_Key_t k, LockHt_Val_t v) {
    uint32_t fullhash = getHash(ht->key_type, k);
    size_t idx = fullhash & (ht->num_entries - 1);
    /*** ACQUIRE LOCK TO THE ENTRY ****/
    omp_lock_t *pEntLock = &ht->locks[idx];

    //acquire lock of the entry-chain head
//    omp_set_lock(&pEntLock);
//    LOG("get the lock!");
    ACQUIRE_LOCK(pEntLock);
    if (ht->entries[idx] == NULL) {
        ht->entries[idx] = LockHt_newEntry(k, v, NULL);
        //release lock
//        omp_unset_lock(pEntLock);
        RELEASE_LOCK(pEntLock);
        return LOCKHT_NIL;
    }

    LockHt_Val_t oldValue;

    //entry head is not null
    LockHt_Entry_t *curEnt = ht->entries[idx];
    while (curEnt->next) {
        //found the key, update the value
        if (keyEqual(ht->key_type, curEnt->key, k)) {
            oldValue = curEnt->val;
            curEnt->val = v;
            //release lock
            //omp_unset_lock(pEntLock);
            RELEASE_LOCK(pEntLock);
            return oldValue;
        }
        //else go next
        curEnt = curEnt->next;
    }


    //we are at the last entry whose next is NULL
    if (keyEqual(ht->key_type, curEnt->key, k)) {
        oldValue = curEnt->val;
        curEnt->val = v;
    } else {
        //it's a new entry
        oldValue = LOCKHT_NIL;
        curEnt->next = LockHt_newEntry(k, v, NULL);
    }

    RELEASE_LOCK(pEntLock);

    //omp_unset_lock(&pEntLock);

    return oldValue;

}

//use course lock 
LockHt_Val_t LockHt_put2(LockHt_Hashtable_t *ht, LockHt_Key_t k, LockHt_Val_t v) {
    uint32_t fullhash = getHash(ht->key_type, k);
    size_t idx = fullhash & (ht->num_entries - 1);

    omp_lock_t* pLock = & ht->locks[0];

    ACQUIRE_LOCK(pLock);

    if (ht->entries[idx] == NULL) {
        ht->entries[idx] = LockHt_newEntry(k, v, NULL);
        RELEASE_LOCK(pLock);
        return LOCKHT_NIL;
    }

    LockHt_Val_t oldValue;

    //entry head is not null
    LockHt_Entry_t *curEnt = ht->entries[idx];
    while (curEnt->next) {
        //found the key, update the value
        if (keyEqual(ht->key_type, curEnt->key, k)) {
            oldValue = curEnt->val;
            curEnt->val = v;
            RELEASE_LOCK(pLock);
            return oldValue;
        }
        //else go next
        curEnt = curEnt->next;
    }


    //we are at the last entry whose next is NULL
    if (keyEqual(ht->key_type, curEnt->key, k)) {
        oldValue = curEnt->val;
        curEnt->val = v;
    } else {
        //it's a new entry
        oldValue = LOCKHT_NIL;
        curEnt->next = LockHt_newEntry(k, v, NULL);
    }


    //omp_unset_lock(&pEntLock);
    RELEASE_LOCK(pLock);
    return oldValue;

}




LockHt_Val_t LockHt_get(LockHt_Hashtable_t *ht, LockHt_Key_t k) {
    uint32_t fullhash = getHash(ht->key_type, k);
    size_t nEnts = ht->num_entries;
    size_t idx = fullhash & (nEnts - 1);
    //returned value, default to NIL
    LockHt_Val_t vRet = LOCKHT_NIL;
    //get lock
    omp_set_lock(&ht->locks[idx]);

    do {
        if (ht->entries[idx] == NULL)
            break;
        LockHt_Entry_t *curEnt = ht->entries[idx];
        while (curEnt && (!keyEqual(ht->key_type, curEnt->key, k)))
            curEnt = curEnt->next;

        if (curEnt == NULL)
            break;
        //else we have a key match
        vRet = curEnt->val;
    } while (0);
    //release lock
    omp_unset_lock(&ht->locks[idx]);

    return vRet;

}

//use course lock 
LockHt_Val_t LockHt_get2(LockHt_Hashtable_t *ht, LockHt_Key_t k)
{
    uint32_t fullhash = getHash(ht->key_type, k);
    size_t nEnts = ht->num_entries;
    size_t idx = fullhash & (nEnts - 1);
    //returned value, default to NIL
    LockHt_Val_t vRet = LOCKHT_NIL;

    omp_lock_t* pLock = & ht->locks[0];
    ACQUIRE_LOCK(pLock);
   do {
            if (ht->entries[idx] == NULL)
                break;
            LockHt_Entry_t *curEnt = ht->entries[idx];
            while (curEnt && (!keyEqual(ht->key_type, curEnt->key, k)))
                curEnt = curEnt->next;

            if (curEnt == NULL)
                break;
            //else we have a key match
            vRet = curEnt->val;
        } while (0);
        //release lock
    RELEASE_LOCK(pLock);
    return vRet;
}


void LockHt_remove(LockHt_Hashtable_t *ht, LockHt_Key_t k) {
    LockHt_put(ht, k, LOCKHT_NIL);
}

int LockHt_hasKey(LockHt_Hashtable_t *ht, LockHt_Key_t k) {
    LockHt_Val_t vRet = LockHt_get(ht, k);
    return vRet != LOCKHT_NIL;

}
