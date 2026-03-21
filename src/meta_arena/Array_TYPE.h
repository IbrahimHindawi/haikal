#pragma once
/* Array_TYPE */
#include <core.h>
#include <saha.h>

#include "TYPE.h"

structdef(Array_TYPE) {
    TYPE *data;
    u64 length;
};

Array_TYPE Array_TYPE_reserve(memops_arena *arena, u64 length);
void Array_TYPE_destroy(memops_arena *arena, Array_TYPE *array);
TYPE *Array_TYPE_at(Array_TYPE *array, u64 index);
int Array_TYPE_is_empty(memops_arena *arena, Array_TYPE *array);
