#pragma once
/* Map_vec3 */
#include <core.h>

structdecl(vec3);
#include "Map_vec3.h"

u64 Map_vec3_hash_key(const char* key);
bool Map_vec3_expand(Arena *arena, Map_vec3* hashmap);
const char *Map_vec3_set_entry(MapEntry_vec3 *entries, usize border, const char *key, vec3 val, usize *plength);

