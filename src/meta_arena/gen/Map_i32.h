#pragma once
/* Map_i32 */
#include <core.h>
#include <saha.h>

primdecl(i32);

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

structdef(MapEntry_i32) {
    const char *key;
    i32 val;
};

structdef(Map_i32) {
    MapEntry_i32 *entries;
    size_t length;
    size_t border;
};

Map_i32 *Map_i32_create(Arena *arena);
void Map_i32_destroy(Arena *arena, Map_i32 *hashmap);
i32 *Map_i32_get(Arena *arena, Map_i32 *hashmap, const char *key);
const char *Map_i32_set(Arena *arena, Map_i32 *hashmap, const char *key, i32 val);
usize Map_i32_length(Arena *arena, Map_i32 *hashmap);

structdef(MapIterator_i32) {
    const char *key;
    i32 val;
    Map_i32 *_hashmap;
    usize _index;
};

MapIterator_i32 MapIterator_i32_create(Arena *arena, Map_i32* hashmap);
bool MapIterator_i32_next(Arena *arena, MapIterator_i32* it);

