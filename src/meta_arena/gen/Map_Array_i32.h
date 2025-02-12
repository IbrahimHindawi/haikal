#pragma once
/* Map_Array_i32 */
#include <core.h>
#include <saha.h>

structdecl(Array_i32);

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

structdef(MapEntry_Array_i32) {
    const char *key;
    Array_i32 val;
};

structdef(Map_Array_i32) {
    MapEntry_Array_i32 *entries;
    size_t length;
    size_t border;
};

Map_Array_i32 *Map_Array_i32_create(Arena *arena);
void Map_Array_i32_destroy(Arena *arena, Map_Array_i32 *hashmap);
Array_i32 *Map_Array_i32_get(Arena *arena, Map_Array_i32 *hashmap, const char *key);
const char *Map_Array_i32_set(Arena *arena, Map_Array_i32 *hashmap, const char *key, Array_i32 val);
usize Map_Array_i32_length(Arena *arena, Map_Array_i32 *hashmap);

structdef(MapIterator_Array_i32) {
    const char *key;
    Array_i32 val;
    Map_Array_i32 *_hashmap;
    usize _index;
};

MapIterator_Array_i32 MapIterator_Array_i32_create(Arena *arena, Map_Array_i32* hashmap);
bool MapIterator_Array_i32_next(Arena *arena, MapIterator_Array_i32* it);

