#pragma once
/* Map_TYPE */
#include <core.h>

#include "TYPE.h"
#include "Map_TYPE.h"

u64 Map_TYPE_hash_key(const char* key);
bool Map_TYPE_expand(Arena *arena, Map_TYPE* hashmap);
const char *Map_TYPE_set_entry(MapEntry_TYPE *entries, usize border, const char *key, TYPE val, usize *plength);
