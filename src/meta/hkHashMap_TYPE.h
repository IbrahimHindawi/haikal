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

static u64 hkHashMap_TYPE_hash_key(const char* key);
static bool hkHashMap_TYPE_expand(hkHashMap_TYPE* hashmap);
static const char *hkHashMap_TYPE_set_entry(hkHashMapEntry_TYPE *entries, usize border, const char *key, TYPE val, usize *plength);
hkHashMap_TYPE *hkHashMap_TYPE_create();
void hkHashMap_TYPE_destroy(hkHashMap_TYPE *hashmap);
TYPE *hkHashMap_TYPE_get(hkHashMap_TYPE *hashmap, const char *key);
const char *hkHashMap_TYPE_set(hkHashMap_TYPE *hashmap, const char *key, TYPE val);
usize hkHashMap_TYPE_length(hkHashMap_TYPE *hashmap);
hkHashMapIterator_TYPE hkHashMapIterator_TYPE_create(hkHashMap_TYPE* hashmap);
bool hkHashMapIterator_TYPE_next(hkHashMapIterator_TYPE* it);

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static u64 hkHashMap_TYPE_hash_key(const char* key) {
    u64 hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (u64)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

static bool hkHashMap_TYPE_expand(hkHashMap_TYPE* hashmap) {
    // Allocate new entries array.
    size_t new_capacity = hashmap->border * 2;
    if (new_capacity < hashmap->border) {
        return false;  // overflow (border would be too big)
    }
    hkHashMapEntry_TYPE *new_entries = calloc(new_capacity, sizeof(hkHashMapEntry_TYPE));
    if (!new_entries) {
        return false;
    }

    // Iterate entries, move all non-empty ones to new hashmap's entries.
    for (size_t i = 0; i < hashmap->border; i++) {
        hkHashMapEntry_TYPE entry = hashmap->entries[i];
        if (entry.key) {
            hkHashMap_TYPE_set_entry(new_entries, new_capacity, entry.key, entry.val, NULL);
        }
    }

    // Free old entries array and update this hashmap's details.
    free(hashmap->entries);
    hashmap->entries = new_entries;
    hashmap->border = new_capacity;
    return true;
}

static const char *hkHashMap_TYPE_set_entry(hkHashMapEntry_TYPE *entries, usize border, const char *key, TYPE val, usize *plength) {
    u64 hash = hkHashMap_TYPE_hash_key(key);
    usize index = (usize)(hash & (u64)(border - 1));
    while (entries[index].key) {
        if (strcmp(key, entries[index].key) == 0) {
            entries[index].val = val;
            return entries[index].key;
        }
        index += 1;
        if (index >= border) {
            index = 0;
        }
    }
    if (plength) {
        key = _strdup(key);
        if (!key) {
            return NULL;
        }
        (*plength)++;
    }
    entries[index].key = (char *)key;
    entries[index].val = val;
    return key;
}


#define hkHashMap_TYPE_initial_capacity 16

hkHashMap_TYPE *hkHashMap_TYPE_create() {
    hkHashMap_TYPE *hashmap = malloc(sizeof(hkHashMap_TYPE));
    if (!hashmap) {
        return NULL;
    }
    hashmap->length = 0;
    hashmap->border = hkHashMap_TYPE_initial_capacity;

    hashmap->entries = calloc(hashmap->border, sizeof(hkHashMapEntry_TYPE));
    if (!hashmap->entries) {
        free(hashmap);
        return NULL;
    }
    return hashmap;
}

void hkHashMap_TYPE_destroy(hkHashMap_TYPE *hashmap) {
    // for (i32 i = 0; i < hashmap->border; i++) {
    //     free((void *)hashmap->entries[i].key);
    // }
    free(hashmap->entries);
    free(hashmap);
}

TYPE *hkHashMap_TYPE_get(hkHashMap_TYPE *hashmap, const char *key) {
    u64 hash = hkHashMap_TYPE_hash_key(key);
    usize index = (usize)(hash & (u64)(hashmap->border - 1));
    while (hashmap->entries[index].key) {
        if (strcmp(key, hashmap->entries[index].key) == 0) {
            return &hashmap->entries[index].val;
        }
        index += 1;
        if (index >= hashmap->border) {
            index = 0;
        }
    }
    return NULL;
}

const char *hkHashMap_TYPE_set(hkHashMap_TYPE *hashmap, const char *key, TYPE val) {
    // assert(val != NULL);
    // if (!val) {
    //     return NULL;
    // }
    if (hashmap->length >= hashmap->border / 2) {
        if(!hkHashMap_TYPE_expand(hashmap)) {
            return NULL;
        }
    }
    return hkHashMap_TYPE_set_entry(hashmap->entries, hashmap->border, key, val, &hashmap->length);
}

usize hkHashMap_TYPE_length(hkHashMap_TYPE *hashmap) {
    return hashmap->length;
}

hkHashMapIterator_TYPE hkHashMapIterator_TYPE_create(hkHashMap_TYPE* hashmap) {
    hkHashMapIterator_TYPE it;
    it._hashmap = hashmap;
    it._index = 0;
    return it;
}

bool hkHashMapIterator_TYPE_next(hkHashMapIterator_TYPE* it) {
    // Loop till we've hit end of entries array.
    hkHashMap_TYPE* hashmap = it->_hashmap;
    while (it->_index < hashmap->border) {
        size_t i = it->_index;
        it->_index++;
        if (hashmap->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and val.
            hkHashMapEntry_TYPE entry = hashmap->entries[i];
            it->key = entry.key;
            it->val = entry.val;
            return true;
        }
    }
    return false;
}
