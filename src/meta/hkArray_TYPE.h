#pragma once
/* hkArray_TYPE */
#include <core.h>

#include "TYPE.h"

structdef(hkArray_TYPE) {
    TYPE *data;
    size_t length;
    size_t border;
};

hkArray_TYPE hkArray_TYPE_create(size_t length);
void hkArray_TYPE_destroy(hkArray_TYPE *array);
TYPE *hkArray_TYPE_resize(hkArray_TYPE *array, size_t new_border);
TYPE *hkArray_TYPE_append(hkArray_TYPE *array, TYPE elem);
int hkArray_TYPE_is_empty(hkArray_TYPE *array);
