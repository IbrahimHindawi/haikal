#pragma once
/* Map_Rec */
#include <core.h>
#include <saha.h>

structdecl(Rec);

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

structdef(MapEntry_Rec) {
    const char *key;
    Rec val;
};

structdef(Map_Rec) {
    MapEntry_Rec *entries;
    size_t length;
    size_t border;
};

Map_Rec *Map_Rec_create(Arena *arena);
void Map_Rec_destroy(Arena *arena, Map_Rec *hashmap);
Rec *Map_Rec_get(Arena *arena, Map_Rec *hashmap, const char *key);
const char *Map_Rec_set(Arena *arena, Map_Rec *hashmap, const char *key, Rec val);
usize Map_Rec_length(Arena *arena, Map_Rec *hashmap);

structdef(MapIterator_Rec) {
    const char *key;
    Rec val;
    Map_Rec *_hashmap;
    usize _index;
};

MapIterator_Rec MapIterator_Rec_create(Arena *arena, Map_Rec* hashmap);
bool MapIterator_Rec_next(Arena *arena, MapIterator_Rec* it);

