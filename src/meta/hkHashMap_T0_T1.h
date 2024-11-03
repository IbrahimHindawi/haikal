#pragma once
/* hkHashMap_T0_T1 */
#include <core.h>

#include "TYPE.h"

structdef(hkHashMapEntry_T0_T1) {
    T0 key;
    T1 val;
};

structdef(hkHashMap_T0_T1) {
    hkHashMapEntry_T0_T1 *entries;
    size_t length;
    size_t border;
};

#define hkHashMap_T0_T1_initial_capacity 16

// hkHashMap_T0_T1 hkHashMap_T0_T1_create(size_t length);

hkHashMap_T0_T1 *hkHashMap_T0_T1_create() {
    hkHashMap_T0_T1 *hashmap = malloc(sizeof(hkHashMap_T0_T1));
    if (!hashmap) {
        return NULL;
    }
    hashmap->length = 0;
    hashmap->border = hkHashMap_T0_T1_initial_capacity;

    hashmap->entries = calloc(hashmap->border, sizeof(hkHashMapEntry_T0_T1));
    if (!hashmap->entries) {
        free(hashmap);
        return NULL;
    }
    return hashmap;
}

void hkHashMap_T0_T1_destroy(hkHashMap_T0_T1 *hashmap) {
    // for (i32 i = 0; i < hashmap->border; i++) {
    //     free((void *)hashmap->entries[i].key);
    // }
    free(hashmap->entries);
    free(hashmap);
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}
