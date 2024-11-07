#pragma once
/* hkHashMap_TYPE */
#include <core.h>

#include "TYPE.h"

structdef(hkHashMapEntry_TYPE) {
    const char *key;
    TYPE val;
};

structdef(hkHashMap_TYPE) {
    hkHashMapEntry_TYPE *entries;
    size_t length;
    size_t border;
};

structdef(hkHashMapIterator_TYPE) {
    const char *key;
    TYPE val;
    hkHashMap_TYPE *_hashmap;
    usize _index;
};

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

u64 hkHashMap_TYPE_hash_key(const char* key);
bool hkHashMap_TYPE_expand(hkHashMap_TYPE* hashmap);
const char *hkHashMap_TYPE_set_entry(hkHashMapEntry_TYPE *entries, usize border, const char *key, TYPE val, usize *plength);
hkHashMap_TYPE *hkHashMap_TYPE_create();
void hkHashMap_TYPE_destroy(hkHashMap_TYPE *hashmap);
TYPE *hkHashMap_TYPE_get(hkHashMap_TYPE *hashmap, const char *key);
const char *hkHashMap_TYPE_set(hkHashMap_TYPE *hashmap, const char *key, TYPE val);
usize hkHashMap_TYPE_length(hkHashMap_TYPE *hashmap);
hkHashMapIterator_TYPE hkHashMapIterator_TYPE_create(hkHashMap_TYPE* hashmap);
bool hkHashMapIterator_TYPE_next(hkHashMapIterator_TYPE* it);
