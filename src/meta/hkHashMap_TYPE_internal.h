#pragma once
/* hkHashMap_TYPE */
#include <core.h>

#include "TYPE.h"
#include "hkHashMap_TYPE.h"

u64 hkHashMap_TYPE_hash_key(const char* key);
bool hkHashMap_TYPE_expand(hkHashMap_TYPE* hashmap);
const char *hkHashMap_TYPE_set_entry(hkHashMapEntry_TYPE *entries, usize border, const char *key, TYPE val, usize *plength);
