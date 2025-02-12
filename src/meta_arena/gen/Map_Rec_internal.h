#pragma once
/* Map_Rec */
#include <core.h>

structdecl(Rec);
#include "Map_Rec.h"

u64 Map_Rec_hash_key(const char* key);
bool Map_Rec_expand(Arena *arena, Map_Rec* hashmap);
const char *Map_Rec_set_entry(MapEntry_Rec *entries, usize border, const char *key, Rec val, usize *plength);

