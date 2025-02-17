#include <core.h>

#include "Array_TYPE.h"

Array_TYPE Array_TYPE_reserve(u64 length) {
    Array_TYPE array = {0};
    if (length == 0) {
        return array;
    }
    array.length = length;
    array.data = malloc(sizeof(TYPE) * length);
    if (array.data == NULL) {
        printf("haikal::Memory Allocation Failure!\n");
        exit(-1);
    }
    // #ifdef DEBUG
    memset(array.data, 0, sizeof(TYPE) * length);
    // #endif
    array.border = length;
    return array;
}

void Array_TYPE_destroy(Array_TYPE *array) {
    array->border = 0;
    array->length = 0;
    free(array->data);
    array->data = NULL;
}

TYPE *Array_TYPE_resize(Array_TYPE *array) {
    u64 old_border = array->border;
    array->border *= 2;
    array->data = realloc(array->data, sizeof(TYPE) * array->border);
    if (array->data == NULL) {
        printf("haikal::Memory Reallocation Failure!\n");
        exit(-1);
    }
    // #ifdef DEBUG
    if (array->border > old_border) {
        u64 border_difference = array->border - old_border;
        char *cursor = (char *)array->data;
        memset(cursor + (sizeof(TYPE) * old_border), 0, sizeof(TYPE) * border_difference);
    }
    // #endif
    return array->data;
}

TYPE *Array_TYPE_append(Array_TYPE *array, TYPE elem) {
    if (array->length == 0 && array->border == 0) { 
        array->length += 1;
        array->border += 1;
        array->data = realloc(NULL, sizeof(TYPE) * array->border);
        array->data[array->length - 1] = elem;
        // char *cursor = (char *)array->data;
        // memcpy(cursor, elem, array->unit_size);
        return array->data;
    }
    if (array->length == array->border ) {
        array->border *= 2;
        // TODO: check if it zeroes out properly
        Array_TYPE_resize(array);
    }
    array->length += 1;
    array->data[array->length - 1] = elem;
    // char *cursor = (char *)array->data;
    // memcpy(cursor + ((array->length - 1) * array->unit_size), elem, array->unit_size);
    return &array->data[array->length - 1];
}

int Array_TYPE_is_empty(Array_TYPE *array) {
    return array->length == 0 ? 1 : 0;
}

// // Function to calculate the next power of two
// unsigned int nextPowerOfTwo(unsigned int n) {
//     if (n == 0) return 1;  // Special case for 0
//     n--;                   // Decrement n to handle cases where n is already a power of two
//     n |= n >> 1;
//     n |= n >> 2;
//     n |= n >> 4;
//     n |= n >> 8;
//     n |= n >> 16;
//     return n + 1;          // Add 1 to get the next power of two
// }

