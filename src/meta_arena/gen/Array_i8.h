#pragma once
/* Array_i8 */
#include <core.h>
#include <saha.h>

primdecl(i8);

structdef(Array_i8) {
    i8 *data;
    u64 length;
    u64 border;
};

Array_i8 Array_i8_reserve(Arena *arena, u64 length);
void Array_i8_destroy(Arena *arena, Array_i8 *array);
i8 *Array_i8_resize(Arena *arena, Array_i8 *array);
i8 *Array_i8_append(Arena *arena, Array_i8 *array, i8 elem);
int Array_i8_is_empty(Arena *arena, Array_i8 *array);

