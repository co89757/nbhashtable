#include <stdlib.h>
#include <string.h>
#include "nstring.h"


const Type_t TYPE_NSTRING = { 
	(compare_fun_t) ns_compare, 
	(hash_fun_t)ns_hash, 
	(clone_fun_t) ns_dup };

nstring_t* ns_new(uint32_t len)
{
	nstring_t* ns = (nstring_t*) malloc(sizeof(nstring_t) + len);
	ns->len = len;
	return ns;
}

nstring_t* ns_init(const char* str)
{
	int len = strlen(str) ;
    int size = len + 1;
	nstring_t* pString =  ns_new(size);
	strncpy(pString->data,str,size);
	return pString;

}

int ns_compare(const nstring_t* ns1, const nstring_t* ns2)
{
	int d = memcmp(ns1->data, ns2->data, (ns1->len < ns2->len) ? ns1->len : ns1->len);
    return (d == 0) ? ns1->len - ns2->len : d;
}


uint32_t ns_hash(const nstring_t* ns)
{
 	return murmur32(ns->data, ns->len);
}


nstring_t* ns_dup(const nstring_t* ns1)
{
	nstring_t *ns2 = ns_new(ns1->len);
    memcpy(ns2->data, ns1->data, ns1->len);
    return ns2;
}