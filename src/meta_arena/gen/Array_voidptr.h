#pragma once
/* Array_voidptr */
#include <core.h>
#include <saha.h>

primdecl(voidptr);

structdef(Array_voidptr) {
    voidptr *data;
    u64 length;
    u64 border;
};

Array_voidptr Array_voidptr_reserve(Arena *arena, u64 length);
void Array_voidptr_destroy(Arena *arena, Array_voidptr *array);
voidptr *Array_voidptr_resize(Arena *arena, Array_voidptr *array);
voidptr *Array_voidptr_append(Arena *arena, Array_voidptr *array, voidptr elem);
int Array_voidptr_is_empty(Arena *arena, Array_voidptr *array);

