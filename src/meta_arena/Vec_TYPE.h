#pragma once
/* Vec_TYPE */
#include <core.h>
#include <saha.h>

#include "TYPE.h"

structdef(Vec_TYPE) {
    TYPE *data;
    u64 length;
    u64 border;
};

Vec_TYPE Vec_TYPE_reserve(memops_arena *arena, u64 length);
void Vec_TYPE_destroy(memops_arena *arena, Vec_TYPE *array);
TYPE *Vec_TYPE_resize(memops_arena *arena, Vec_TYPE *array);
TYPE *Vec_TYPE_append(memops_arena *arena, Vec_TYPE *array, TYPE elem);
int Vec_TYPE_is_empty(memops_arena *arena, Vec_TYPE *array);
