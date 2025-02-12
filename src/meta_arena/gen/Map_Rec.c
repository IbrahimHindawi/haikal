#include <core.h>

#include "Map_Rec.h"
#include "Map_Rec_internal.h"

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
u64 Map_Rec_hash_key(const char* key) {
    u64 hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (u64)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

bool Map_Rec_expand(Arena *arena, Map_Rec* hashmap) {
    // Allocate new entries array.
    size_t new_capacity = hashmap->border * 2;
    if (new_capacity < hashmap->border) {
        return false;  // overflow (border would be too big)
    }
    // MapEntry_Rec *new_entries = calloc(new_capacity, sizeof(MapEntry_Rec));
    MapEntry_Rec *new_entries = arenaPushArray(arena, MapEntry_Rec, new_capacity);
    if (!new_entries) {
        return false;
    }

    // Iterate entries, move all non-empty ones to new hashmap's entries.
    for (size_t i = 0; i < hashmap->border; i++) {
        MapEntry_Rec entry = hashmap->entries[i];
        if (entry.key) {
            Map_Rec_set_entry(new_entries, new_capacity, entry.key, entry.val, NULL);
        }
    }

    // Free old entries array and update this hashmap's details.
    // free(hashmap->entries);
    hashmap->entries = new_entries;
    hashmap->border = new_capacity;
    return true;
}

const char *Map_Rec_set_entry(MapEntry_Rec *entries, usize border, const char *key, Rec val, usize *plength) {
    u64 hash = Map_Rec_hash_key(key);
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


#define Map_Rec_initial_capacity 16

Map_Rec *Map_Rec_create(Arena *arena) {
    // Map_Rec *hashmap = malloc(sizeof(Map_Rec));
    Map_Rec *hashmap = arenaPushArray(arena, Map_Rec, 1);
    if (!hashmap) {
        return NULL;
    }
    hashmap->length = 0;
    hashmap->border = Map_Rec_initial_capacity;

    // hashmap->entries = calloc(hashmap->border, sizeof(MapEntry_Rec));
    hashmap->entries = arenaPushArray(arena, MapEntry_Rec, hashmap->border);
    if (!hashmap->entries) {
        // free(hashmap);
        return NULL;
    }
    return hashmap;
}

void Map_Rec_destroy(Arena *arena, Map_Rec *hashmap) {
    // for (i32 i = 0; i < hashmap->border; i++) {
    //     free((void *)hashmap->entries[i].key);
    // }
    // free(hashmap->entries);
    // free(hashmap);
}

Rec *Map_Rec_get(Arena *arena, Map_Rec *hashmap, const char *key) {
    u64 hash = Map_Rec_hash_key(key);
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

const char *Map_Rec_set(Arena *arena, Map_Rec *hashmap, const char *key, Rec val) {
    // assert(val != NULL);
    // if (!val) {
    //     return NULL;
    // }
    if (hashmap->length >= hashmap->border / 2) {
        if(!Map_Rec_expand(arena, hashmap)) {
            return NULL;
        }
    }
    return Map_Rec_set_entry(hashmap->entries, hashmap->border, key, val, &hashmap->length);
}

usize Map_Rec_length(Arena *arena, Map_Rec *hashmap) {
    return hashmap->length;
}

MapIterator_Rec MapIterator_Rec_create(Arena *arena, Map_Rec* hashmap) {
    MapIterator_Rec it = {0};
    if (hashmap) {
        it._hashmap = hashmap;
        it._index = 0;
    }
    return it;
}

bool MapIterator_Rec_next(Arena *arena, MapIterator_Rec* it) {
    // Loop till we've hit end of entries array.
    Map_Rec* hashmap = it->_hashmap;
    while (it->_index < hashmap->border) {
        size_t i = it->_index;
        it->_index++;
        if (hashmap->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and val.
            MapEntry_Rec entry = hashmap->entries[i];
            it->key = entry.key;
            it->val = entry.val;
            return true;
        }
    }
    return false;
}

