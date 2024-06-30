#pragma once
/* hkArray_TYPE */
#include <core.h>

#include "TYPE.h"

structdef(hkArray_TYPE) {
    TYPE *data;
    size_t unit_size;
    size_t length;
    size_t border;
};

hkArray_TYPE hkarray_TYPE_create(size_t length);
void hkarray_TYPE_destroy(hkArray_TYPE *array);
TYPE *hkarray_TYPE_resize(hkArray_TYPE *array, size_t new_border);
TYPE *hkarray_TYPE_append(hkArray_TYPE *array, TYPE elem);
int hkarray_TYPE_is_empty(hkArray_TYPE *array);

#ifdef HKARRAY_IMPL

hkArray_TYPE hkarray_TYPE_create(size_t length) {
    hkArray_TYPE array = {0};
    if (length == 0) {
        array.unit_size = sizeof(TYPE);
        array.length = length;
        array.data = NULL;
        array.border = 0;
        return array;
    }
    array.unit_size = sizeof(TYPE);
    array.length = length;
    array.data = malloc(array.unit_size * length);
    if (array.data == NULL) {
        printf("Memory Allocation Failure!");
        exit(-1);
    }
    // #ifdef DEBUG
    memset(array.data, 0, array.unit_size * length);
    // #endif
    array.border = length;
    return array;
}

void hkarray_TYPE_destroy(hkArray_TYPE *array) {
    array->border = 0;
    array->length = 0;
    array->unit_size = 0;
    free(array->data);
}

// Resize border: Should be a private function.
TYPE *hkarray_TYPE_resize(hkArray_TYPE *array, size_t new_border) {
    size_t old_border = array->border;
    array->border *= 2;
    // array->border = new_border;
    new_border = array->border;
    array->data = realloc(array->data, array->unit_size * array->border);
    if (array->data == NULL) {
        printf("Memory Reallocation Failure!");
        exit(-1);
    }
    // #ifdef DEBUG
    if (new_border > old_border) {
        size_t border_difference = new_border - old_border;
        char *cursor = (char *)array->data;
        memset(cursor + (array->unit_size * old_border), 0, array->unit_size * border_difference);
    }
    // #endif
    array->border = new_border;
    return array->data;
}

TYPE *hkarray_TYPE_append(hkArray_TYPE *array, TYPE elem) {
    if (array->length == 0 && array->border == 0) { 
        array->length += 1;
        array->border += 1;
        hkarray_TYPE_resize(array, array->border);
        array->data[array->length - 1] = elem;
        // char *cursor = (char *)array->data;
        // memcpy(cursor, elem, array->unit_size);
        return array->data;
    }
    if (array->length == array->border ) {
        array->border *= 2;
        // TODO: check if it zeroes out properly
        hkarray_TYPE_resize(array, array->border);
    }
    array->length += 1;
    array->data[array->length - 1] = elem;
    // char *cursor = (char *)array->data;
    // memcpy(cursor + ((array->length - 1) * array->unit_size), elem, array->unit_size);
    return array->data;
}

int hkarray_TYPE_is_empty(hkArray_TYPE *array) {
    return array->length == 0 ? 1 : 0;
}

#endif
