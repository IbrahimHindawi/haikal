#pragma once
/* hkArray_f64 */
#include "../core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



structdef(hkArray_f64) {
    f64 *data;
    size_t unit_size;
    size_t length;
    size_t border;
};

hkArray_f64 hkarray_f64_create(size_t length) {
    hkArray_f64 array = {0};
    array.unit_size = sizeof(f64);
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

void hkarray_f64_destroy(hkArray_f64 *array) {
    array->border = 0;
    array->length = 0;
    array->unit_size = 0;
    free(array->data);
}

// Resize border: Should be a private function.
void *hkarray_f64_resize(hkArray_f64 *array, size_t new_border) {
    size_t old_border = array->border;
    array->border = new_border;
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

void *hkarray_f64_append(hkArray_f64 *array, void *elem) {
    if (array->length == 0 && array->border == 0) { 
        array->length += 1;
        array->border += 1;
        hkarray_f64_resize(array, array->border);
        char *cursor = (char *)array->data;
        memcpy(cursor, elem, array->unit_size);
        return array->data;
    }
    if (array->length == array->border ) {
        array->border *= 2;
        hkarray_f64_resize(array, array->border);
    }
    array->length += 1;
    char *cursor = (char *)array->data;
    memcpy(cursor + ((array->length - 1) * array->unit_size), elem, array->unit_size);
    return array->data;
}

int hkarray_f64_is_empty(hkArray_f64 *array) {
    return array->length == 0 ? 1 : 0;
}

/*
void array_print_(hkArray *array, int iterator, const char *fmt, ...) {
    printf("metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n",
            array->data, array->unit_size, array->length, array->border);
    printf("components: { ");

    va_list args;
    va_start(args, iterator);

    for (iterator  = 0; iterator < array->length; iterator++) {
        // printf("%d, ", *iterator);
        // va_list args;
        // vec3f vector = va_arg(args, iterator);
        // va_start(args, *iterator);
        // vprintf(fmt, args);
        // va_end(args);
    }
//    va_list args;
//    va_start(args, 0);
//    while(arg = va_arg(args, int) < array->length) {
//        vprintf(fmt, arg)
//    }
//    va_end(args);
    printf("\b\b}\n\n");
}
*/

/*
void array_print__(hkArray *array, int length, const char *fmt, ...) {
    printf("metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n",
            array->data, array->unit_size, array->length, array->border);
    printf("components: { ");
    va_list args;
    va_start(args, array);
    for (int i  = 0; i < length; i++) {
        int value = va_arg(args, int);
        printf(fmt, i, value);
    }
    va_end(args);
    printf("\b\b}\n\n");
}
*/

