#pragma once
/* hkArray_TYPE */
#include <core.h>

#include "TYPE.h"

structdef(hkArray_TYPE) {
    TYPE *data;
    u64 length;
    u64 border;
};

hkArray_TYPE hkArray_TYPE_create(u64 length);
void hkArray_TYPE_destroy(hkArray_TYPE *array);
TYPE *hkArray_TYPE_resize(hkArray_TYPE *array);
TYPE *hkArray_TYPE_append(hkArray_TYPE *array, TYPE elem);
int hkArray_TYPE_is_empty(hkArray_TYPE *array);
