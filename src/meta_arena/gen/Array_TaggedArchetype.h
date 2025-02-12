#pragma once
/* Array_TaggedArchetype */
#include <core.h>
#include <saha.h>

structdecl(TaggedArchetype);

structdef(Array_TaggedArchetype) {
    TaggedArchetype *data;
    u64 length;
    u64 border;
};

Array_TaggedArchetype Array_TaggedArchetype_reserve(Arena *arena, u64 length);
void Array_TaggedArchetype_destroy(Arena *arena, Array_TaggedArchetype *array);
TaggedArchetype *Array_TaggedArchetype_resize(Arena *arena, Array_TaggedArchetype *array);
TaggedArchetype *Array_TaggedArchetype_append(Arena *arena, Array_TaggedArchetype *array, TaggedArchetype elem);
int Array_TaggedArchetype_is_empty(Arena *arena, Array_TaggedArchetype *array);

