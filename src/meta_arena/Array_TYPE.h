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

Array_TYPE Array_TYPE_reserve(memops_arena *arena, u64 length);
void Array_TYPE_destroy(memops_arena *arena, Array_TYPE *array);
TYPE *Array_TYPE_resize(memops_arena *arena, Array_TYPE *array);
TYPE *Array_TYPE_append(memops_arena *arena, Array_TYPE *array, TYPE elem);
int Array_TYPE_is_empty(memops_arena *arena, Array_TYPE *array);
