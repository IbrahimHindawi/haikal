#include <core.h>

#include "Map_Array_i32.h"
#include "Map_Array_i32_internal.h"

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
u64 Map_Array_i32_hash_key(const char* key) {
    u64 hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (u64)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

bool Map_Array_i32_expand(Arena *arena, Map_Array_i32* hashmap) {
    // Allocate new entries array.
    size_t new_capacity = hashmap->border * 2;
    if (new_capacity < hashmap->border) {
        return false;  // overflow (border would be too big)
    }
    // MapEntry_Array_i32 *new_entries = calloc(new_capacity, sizeof(MapEntry_Array_i32));
    MapEntry_Array_i32 *new_entries = arenaPushArray(arena, MapEntry_Array_i32, new_capacity);
    if (!new_entries) {
        return false;
    }

    // Iterate entries, move all non-empty ones to new hashmap's entries.
    for (size_t i = 0; i < hashmap->border; i++) {
        MapEntry_Array_i32 entry = hashmap->entries[i];
        if (entry.key) {
            Map_Array_i32_set_entry(new_entries, new_capacity, entry.key, entry.val, NULL);
        }
    }

    // Free old entries array and update this hashmap's details.
    // free(hashmap->entries);
    hashmap->entries = new_entries;
    hashmap->border = new_capacity;
    return true;
}

const char *Map_Array_i32_set_entry(MapEntry_Array_i32 *entries, usize border, const char *key, Array_i32 val, usize *plength) {
    u64 hash = Map_Array_i32_hash_key(key);
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


#define Map_Array_i32_initial_capacity 16

Map_Array_i32 *Map_Array_i32_create(Arena *arena) {
    // Map_Array_i32 *hashmap = malloc(sizeof(Map_Array_i32));
    Map_Array_i32 *hashmap = arenaPushArray(arena, Map_Array_i32, 1);
    if (!hashmap) {
        return NULL;
    }
    hashmap->length = 0;
    hashmap->border = Map_Array_i32_initial_capacity;

    // hashmap->entries = calloc(hashmap->border, sizeof(MapEntry_Array_i32));
    hashmap->entries = arenaPushArray(arena, MapEntry_Array_i32, hashmap->border);
    if (!hashmap->entries) {
        // free(hashmap);
        return NULL;
    }
    return hashmap;
}

void Map_Array_i32_destroy(Arena *arena, Map_Array_i32 *hashmap) {
    // for (i32 i = 0; i < hashmap->border; i++) {
    //     free((void *)hashmap->entries[i].key);
    // }
    // free(hashmap->entries);
    // free(hashmap);
}

Array_i32 *Map_Array_i32_get(Arena *arena, Map_Array_i32 *hashmap, const char *key) {
    u64 hash = Map_Array_i32_hash_key(key);
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

const char *Map_Array_i32_set(Arena *arena, Map_Array_i32 *hashmap, const char *key, Array_i32 val) {
    // assert(val != NULL);
    // if (!val) {
    //     return NULL;
    // }
    if (hashmap->length >= hashmap->border / 2) {
        if(!Map_Array_i32_expand(arena, hashmap)) {
            return NULL;
        }
    }
    return Map_Array_i32_set_entry(hashmap->entries, hashmap->border, key, val, &hashmap->length);
}

usize Map_Array_i32_length(Arena *arena, Map_Array_i32 *hashmap) {
    return hashmap->length;
}

MapIterator_Array_i32 MapIterator_Array_i32_create(Arena *arena, Map_Array_i32* hashmap) {
    MapIterator_Array_i32 it = {0};
    if (hashmap) {
        it._hashmap = hashmap;
        it._index = 0;
    }
    return it;
}

bool MapIterator_Array_i32_next(Arena *arena, MapIterator_Array_i32* it) {
    // Loop till we've hit end of entries array.
    Map_Array_i32* hashmap = it->_hashmap;
    while (it->_index < hashmap->border) {
        size_t i = it->_index;
        it->_index++;
        if (hashmap->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and val.
            MapEntry_Array_i32 entry = hashmap->entries[i];
            it->key = entry.key;
            it->val = entry.val;
            return true;
        }
    }
    return false;
}

