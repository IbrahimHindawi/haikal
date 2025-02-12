#pragma once
/* Map_TYPE */
#include <core.h>
#include <saha.h>

#include "TYPE.h"

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

structdef(MapEntry_TYPE) {
    const char *key;
    TYPE val;
};

structdef(Map_TYPE) {
    MapEntry_TYPE *entries;
    size_t length;
    size_t border;
};

Map_TYPE *Map_TYPE_create(Arena *arena);
void Map_TYPE_destroy(Arena *arena, Map_TYPE *hashmap);
TYPE *Map_TYPE_get(Arena *arena, Map_TYPE *hashmap, const char *key);
const char *Map_TYPE_set(Arena *arena, Map_TYPE *hashmap, const char *key, TYPE val);
usize Map_TYPE_length(Arena *arena, Map_TYPE *hashmap);

structdef(MapIterator_TYPE) {
    const char *key;
    TYPE val;
    Map_TYPE *_hashmap;
    usize _index;
};

MapIterator_TYPE MapIterator_TYPE_create(Arena *arena, Map_TYPE* hashmap);
bool MapIterator_TYPE_next(Arena *arena, MapIterator_TYPE* it);
