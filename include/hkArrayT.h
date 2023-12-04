/*
 * hkArrayT
 * T = type
 * S = struct
 * Must define T before including header.
 * Must not #pragma once
 * Must not header guard
 */

#ifndef T
    #error "Template typename T is undefined in file hkArrayT.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "core.h"

// #define T int
#define S CAT(hkArray,T)

typedef struct S {
    T *data;
    size_t unit_size;
    size_t length;
    size_t border;
} S;

/*
 * create: create a newly allocated data
 * destroy: destroy existing data
 * append: append item at the end of data, possibly double memory
 * prepend: prepend item at the beginning of data
 * insert: insert item in place of item
 * remove: remove item from data, shrink memory
 * resize: resize data to desired size
 * consolidate: fit memory border into length
 * slice: create a newly allocated data from existing data slice
 * is_empty: check if array has 0 items
 * print: display data to console
*/

#define Create CAT(S, Create)
inline S Create(size_t length) {
    S array = {0};
    array.unit_size = sizeof(*array.data);
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

#define Destroy CAT(S, Destroy)
inline void Destroy(S *array) {
    array->border = 0;
    array->length = 0;
    array->unit_size = 0;
    free(array->data);
}

// Resize border: Should be a private function.
#define Resize CAT(S, Resize)
inline T *Resize(S *array, size_t new_border) {
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

#define Append CAT(S, Append)
inline T *Append(S *array, T *elem) {
    if (array->length == 0 && array->border == 0) { 
        array->length += 1;
        array->border += 1;
        Resize(array, array->border);
        char *cursor = (char *)array->data;
        memcpy(cursor, elem, array->unit_size);
        return array->data;
    }
    if (array->length == array->border ) {
        array->border *= 2;
        Resize(array, array->border);
    }
    array->length += 1;
    char *cursor = (char *)array->data;
    memcpy(cursor + ((array->length - 1) * array->unit_size), elem, array->unit_size);
    return array->data;
}

#define IsEmpty CAT(S, IsEmpty)
inline int IsEmpty(S *array) {
    return array->length == 0 ? 1 : 0;
}

#define hkArrayPrint(array, handle, fmt) { \
    printf("metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n", \
            (array).data, (array).unit_size, (array).length, (array).border); \
    printf("components: { "); \
    for (int n  = 0; n < (array).length; n++) { \
        printf( fmt " ", handle[n]); \
    } \
    printf("}\n\n"); \
} \

#define hkArrayPrintStruct(array, handle, fmt) { \
    printf("metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n", \
            (array).data, (array).unit_size, (array).length, (array).border); \
    printf("components: { "); \
    for (int n  = 0; n < (array).length; n++) { \
        printf( fmt ); \
    } \
    printf("\b\b}\n\n"); \
} \

// void array_print_(S *array, int iterator, const char *fmt, ...);

#undef T
#undef S
#undef Create
#undef Destroy
#undef Resize
#undef Append
#undef IsEmpty
