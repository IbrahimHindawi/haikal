#pragma once
/* Map_TYPE */
#include <core.h>

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

Map_TYPE *Map_TYPE_create();
void Map_TYPE_destroy(Map_TYPE *hashmap);
TYPE *Map_TYPE_get(Map_TYPE *hashmap, const char *key);
const char *Map_TYPE_set(Map_TYPE *hashmap, const char *key, TYPE val);
usize Map_TYPE_length(Map_TYPE *hashmap);

structdef(MapIterator_TYPE) {
    const char *key;
    TYPE val;
    Map_TYPE *_hashmap;
    usize _index;
};

MapIterator_TYPE MapIterator_TYPE_create(Map_TYPE* hashmap);
bool MapIterator_TYPE_next(MapIterator_TYPE* it);
