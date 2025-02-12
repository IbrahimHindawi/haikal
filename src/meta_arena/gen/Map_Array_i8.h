#pragma once
/* Map_Array_i8 */
#include <core.h>
#include <saha.h>

structdecl(Array_i8);

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

structdef(MapEntry_Array_i8) {
    const char *key;
    Array_i8 val;
};

structdef(Map_Array_i8) {
    MapEntry_Array_i8 *entries;
    size_t length;
    size_t border;
};

Map_Array_i8 *Map_Array_i8_create(Arena *arena);
void Map_Array_i8_destroy(Arena *arena, Map_Array_i8 *hashmap);
Array_i8 *Map_Array_i8_get(Arena *arena, Map_Array_i8 *hashmap, const char *key);
const char *Map_Array_i8_set(Arena *arena, Map_Array_i8 *hashmap, const char *key, Array_i8 val);
usize Map_Array_i8_length(Arena *arena, Map_Array_i8 *hashmap);

structdef(MapIterator_Array_i8) {
    const char *key;
    Array_i8 val;
    Map_Array_i8 *_hashmap;
    usize _index;
};

MapIterator_Array_i8 MapIterator_Array_i8_create(Arena *arena, Map_Array_i8* hashmap);
bool MapIterator_Array_i8_next(Arena *arena, MapIterator_Array_i8* it);

