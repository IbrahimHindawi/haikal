#pragma once
/* Map_Array_i8 */
#include <core.h>

structdecl(Array_i8);
#include "Map_Array_i8.h"

u64 Map_Array_i8_hash_key(const char* key);
bool Map_Array_i8_expand(Arena *arena, Map_Array_i8* hashmap);
const char *Map_Array_i8_set_entry(MapEntry_Array_i8 *entries, usize border, const char *key, Array_i8 val, usize *plength);

