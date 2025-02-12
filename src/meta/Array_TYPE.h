#pragma once
/* Array_TYPE */
#include <core.h>

#include "TYPE.h"

structdef(Array_TYPE) {
    TYPE *data;
    u64 length;
    u64 border;
};

Array_TYPE Array_TYPE_reserve(u64 length);
void Array_TYPE_destroy(Array_TYPE *array);
TYPE *Array_TYPE_resize(Array_TYPE *array);
TYPE *Array_TYPE_append(Array_TYPE *array, TYPE elem);
int Array_TYPE_is_empty(Array_TYPE *array);
