#pragma once
/* Array_i32 */
#include <core.h>
#include <saha.h>

primdecl(i32);

structdef(Array_i32) {
    i32 *data;
    u64 length;
    u64 border;
};

Array_i32 Array_i32_reserve(Arena *arena, u64 length);
void Array_i32_destroy(Arena *arena, Array_i32 *array);
i32 *Array_i32_resize(Arena *arena, Array_i32 *array);
i32 *Array_i32_append(Arena *arena, Array_i32 *array, i32 elem);
int Array_i32_is_empty(Arena *arena, Array_i32 *array);

