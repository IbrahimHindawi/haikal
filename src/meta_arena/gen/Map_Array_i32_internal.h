#pragma once
/* Map_Array_i32 */
#include <core.h>

structdecl(Array_i32);
#include "Map_Array_i32.h"

u64 Map_Array_i32_hash_key(const char* key);
bool Map_Array_i32_expand(Arena *arena, Map_Array_i32* hashmap);
const char *Map_Array_i32_set_entry(MapEntry_Array_i32 *entries, usize border, const char *key, Array_i32 val, usize *plength);

