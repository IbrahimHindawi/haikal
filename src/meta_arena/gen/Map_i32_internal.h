#pragma once
/* Map_i32 */
#include <core.h>

primdecl(i32);
#include "Map_i32.h"

u64 Map_i32_hash_key(const char* key);
bool Map_i32_expand(Arena *arena, Map_i32* hashmap);
const char *Map_i32_set_entry(MapEntry_i32 *entries, usize border, const char *key, i32 val, usize *plength);

