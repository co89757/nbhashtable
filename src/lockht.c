//
// Created by colin on 12/1/15.
//
#include <omp.h>
#include <stdlib.h>
#include "lockht.h"
#include "hashfunc.h"
/** ENTRY API ***/
LockHt_Entry_t* LockHt_newEntry(LockHt_Key_t k, LockHt_Val_t v, LockHt_Entry_t* nxt)
{
    LockHt_Entry_t* pEnt = (LockHt_Entry_t*) malloc(sizeof(LockHt_Entry_t));
    pEnt->key = k;
    pEnt->val = v;
    pEnt->next = nxt;

    return pEnt;
}

void LockHt_freeEntry(LockHt_Entry_t* ent)
{
    FREE(ent);
}


/*** HashTable API *****/

static
int keyEqual(Type_t* type, LockHt_Key_t key1, LockHt_Key_t key2)
{
    if(type==NULL)
        return key1 == key2;
    return type->compare(GET_PTR(key1), GET_PTR(key2)) == 0 ;
}

static inline
uint32_t getHash(Type_t* type, LockHt_Key_t key)
{
    if(!type)
        return murmur32_8b(key);
    return type->hash(GET_PTR(key));
}

LockHt_Hashtable_t* LockHt_newHashtable(size_t LogN, Type_t* Ktype)
{
    LockHt_Hashtable_t* pHt = (LockHt_Hashtable_t*) malloc( sizeof(LockHt_Hashtable_t));
    size_t nEnts = 1 << LogN;
    pHt->num_entries = nEnts;
    pHt->key_type = Ktype;
    pHt->entries = (LockHt_Entry_t**) calloc(nEnts, sizeof(LockHt_Entry_t*));
    pHt->locks = (omp_lock_t*) malloc(nEnts*sizeof(omp_lock_t));
    int i;
    //init locks for each entry head
    for (i = 0; i < nEnts; ++i) {
        omp_init_lock(&pHt->locks[i]);
    }

    return pHt;

}

void LockHt_freeHashtable(LockHt_Hashtable_t* ht)
{
    size_t nEnts = ht->num_entries;
    //free entries
    int i;
    for (i = 0; i < nEnts; ++i) {
        LockHt_Entry_t* curEnt = ht->entries[i];
        while(curEnt){
            LockHt_Entry_t* temp_curEnt = curEnt;
            LockHt_Entry_t* nextEnt = ht->entries[i]->next;
            LockHt_freeEntry(temp_curEnt);
            curEnt = nextEnt;
        }
    }
    FREE(ht->entries);

    //free locks
    for ( i = 0; i < nEnts; ++i) {
        omp_destroy_lock(&ht->locks[i]);
    }
    FREE(ht->locks);
}

void LockHt_put(LockHt_Hashtable_t* ht, LockHt_Key_t k, LockHt_Val_t v)
{
    uint32_t fullhash = getHash(ht->key_type,k);
    size_t idx = fullhash & (ht->num_entries - 1) ;
    /*** ACQUIRE LOCK TO THE ENTRY ****/
    omp_lock_t entLock = ht->locks[idx];

    omp_set_lock(&entLock);

    if(ht->entries[idx]==NULL){
        ht->entries[idx] = LockHt_newEntry(k,v,NULL);
        return;
    }
    //entry head is not null
    LockHt_Entry_t* curEnt = ht->entries[idx];
    while(curEnt->next){
        //found the key, update the value
        if(keyEqual(ht->key_type,curEnt->key,k)){
            curEnt->val = v;
            omp_unset_lock(&entLock);
            return;
        }
        //else go next
        curEnt = curEnt->next;
    }
    //we are at the last entry whose next is NULL
    if(keyEqual(ht->key_type,curEnt->key,k)){
        curEnt->val = v;
    }else{
        //insert new entry
        curEnt->next = LockHt_newEntry(k,v,NULL);
    }

    omp_unset_lock(&entLock);


}

LockHt_Val_t LockHt_get(LockHt_Hashtable_t* ht, LockHt_Key_t k)
{
    uint32_t fullhash = getHash(ht->key_type, k);
    size_t nEnts = ht->num_entries;
    size_t idx = fullhash & (nEnts-1) ;
    //returned value, default to NIL
    LockHt_Val_t vRet = LOCKHT_NIL;
    //get lock
    omp_set_lock(&ht->locks[idx]);

    do {
        if(ht->entries[idx] == NULL)
            break;
        LockHt_Entry_t* curEnt = ht->entries[idx];
        while (curEnt && (!keyEqual(ht->key_type, curEnt->key,k)))
            curEnt = curEnt->next;

        if(curEnt==NULL)
            break;
        //else we have a key match
        vRet = curEnt->val;
    } while (0);
    //release lock
    omp_unset_lock(&ht->locks[idx]);

    return vRet;

}

void LockHt_remove(LockHt_Hashtable_t* ht, LockHt_Key_t k)
{
//TODO
}

int LockHt_hasKey(LockHt_Hashtable_t* ht, LockHt_Key_t k)
{//TODO
  return FALSE;

}
