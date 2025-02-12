#pragma once
/* Array_TYPE */
#include <core.h>
#include <saha.h>

#include "TYPE.h"

structdef(Array_TYPE) {
    TYPE *data;
    u64 length;
    u64 border;
};

Array_TYPE Array_TYPE_reserve(Arena *arena, u64 length);
void Array_TYPE_destroy(Arena *arena, Array_TYPE *array);
TYPE *Array_TYPE_resize(Arena *arena, Array_TYPE *array);
TYPE *Array_TYPE_append(Arena *arena, Array_TYPE *array, TYPE elem);
int Array_TYPE_is_empty(Arena *arena, Array_TYPE *array);
