#pragma once
/* Map_vec3 */
#include <core.h>
#include <saha.h>

structdecl(vec3);

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

structdef(MapEntry_vec3) {
    const char *key;
    vec3 val;
};

structdef(Map_vec3) {
    MapEntry_vec3 *entries;
    size_t length;
    size_t border;
};

Map_vec3 *Map_vec3_create(Arena *arena);
void Map_vec3_destroy(Arena *arena, Map_vec3 *hashmap);
vec3 *Map_vec3_get(Arena *arena, Map_vec3 *hashmap, const char *key);
const char *Map_vec3_set(Arena *arena, Map_vec3 *hashmap, const char *key, vec3 val);
usize Map_vec3_length(Arena *arena, Map_vec3 *hashmap);

structdef(MapIterator_vec3) {
    const char *key;
    vec3 val;
    Map_vec3 *_hashmap;
    usize _index;
};

MapIterator_vec3 MapIterator_vec3_create(Arena *arena, Map_vec3* hashmap);
bool MapIterator_vec3_next(Arena *arena, MapIterator_vec3* it);

