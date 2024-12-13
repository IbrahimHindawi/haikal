#include <core.h>

#include "hkArray_TYPE.h"

hkArray_TYPE hkArray_TYPE_create(size_t length) {
    hkArray_TYPE array = {0};
    if (length == 0) {
        array.length = 0;
        array.border = 0;
        array.data = NULL;
        return array;
    }
    array.length = length;
    array.data = malloc(sizeof(TYPE) * length);
    if (array.data == NULL) {
        printf("Memory Allocation Failure!");
        exit(-1);
    }
    // #ifdef DEBUG
    memset(array.data, 0, sizeof(TYPE) * length);
    // #endif
    array.border = length;
    return array;
}

void hkArray_TYPE_destroy(hkArray_TYPE *array) {
    array->border = 0;
    array->length = 0;
    free(array->data);
}

TYPE *hkArray_TYPE_resize(hkArray_TYPE *array, size_t new_border) {
    size_t old_border = array->border;
    array->border *= 2;
    // array->border = new_border;
    new_border = array->border;
    array->data = realloc(array->data, sizeof(TYPE) * array->border);
    if (array->data == NULL) {
        printf("Memory Reallocation Failure!");
        exit(-1);
    }
    // #ifdef DEBUG
    if (new_border > old_border) {
        size_t border_difference = new_border - old_border;
        char *cursor = (char *)array->data;
        memset(cursor + (sizeof(TYPE) * old_border), 0, sizeof(TYPE) * border_difference);
    }
    // #endif
    array->border = new_border;
    return array->data;
}

TYPE *hkArray_TYPE_append(hkArray_TYPE *array, TYPE elem) {
    if (array->length == 0 && array->border == 0) { 
        array->length += 1;
        array->border += 1;
        // hkArray_TYPE_resize(array, array->border);
        array->data = realloc(NULL, sizeof(TYPE) * array->border);
        array->data[array->length - 1] = elem;
        // char *cursor = (char *)array->data;
        // memcpy(cursor, elem, array->unit_size);
        return array->data;
    }
    if (array->length == array->border ) {
        array->border *= 2;
        // TODO: check if it zeroes out properly
        hkArray_TYPE_resize(array, array->border);
    }
    array->length += 1;
    array->data[array->length - 1] = elem;
    // char *cursor = (char *)array->data;
    // memcpy(cursor + ((array->length - 1) * array->unit_size), elem, array->unit_size);
    return array->data;
}

int hkArray_TYPE_is_empty(hkArray_TYPE *array) {
    return array->length == 0 ? 1 : 0;
}
