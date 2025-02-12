#include <core.h>

#include "Array_i8.h"

Array_i8 Array_i8_reserve(Arena *arena, u64 length) {
    Array_i8 array = {0};
    if (length == 0) {
        return array;
    }
    array.length = length;
    array.data = arenaPushArray(arena, i8, length);
    // array.data = malloc(sizeof(i8) * length);
    if (array.data == NULL) {
        printf("haikal::Memory Allocation Failure!\n");
        exit(-1);
    }
    // #ifdef DEBUG
    memset(array.data, 0, sizeof(i8) * length);
    // #endif
    array.border = length;
    return array;
}

void Array_i8_destroy(Arena *arena, Array_i8 *array) {
    array->border = 0;
    array->length = 0;
    // free(array->data);
    array->data = NULL;
}

i8 *Array_i8_resize(Arena *arena, Array_i8 *array) {
    u64 old_border = array->border;
    array->border *= 2;
    array->data = arenaRealloc(arena, i8, array->border, array->data, old_border);
    // array->data = realloc(arena, array->data, sizeof(i8) * array->border);
    if (array->data == NULL) {
        printf("haikal::Memory Reallocation Failure!\n");
        exit(-1);
    }
    // #ifdef DEBUG
    if (array->border > old_border) {
        u64 border_difference = array->border - old_border;
        char *cursor = (char *)array->data;
        memset(cursor + (sizeof(i8) * old_border), 0, sizeof(i8) * border_difference);
    }
    // #endif
    return array->data;
}

i8 *Array_i8_append(Arena *arena, Array_i8 *array, i8 elem) {
    if (array->length == 0 && array->border == 0) { 
        array->length += 1;
        array->border += 1;
        array->data = arenaPushArray(arena, i8, array->border);
        // array->data = realloc(NULL, sizeof(i8) * array->border);
        array->data[array->length - 1] = elem;
        // char *cursor = (char *)array->data;
        // memcpy(cursor, elem, array->unit_size);
        return array->data;
    }
    if (array->length == array->border ) {
        array->border *= 2;
        // TODO: check if it zeroes out properly
        Array_i8_resize(arena, array);
    }
    array->length += 1;
    array->data[array->length - 1] = elem;
    // char *cursor = (char *)array->data;
    // memcpy(cursor + ((array->length - 1) * array->unit_size), elem, array->unit_size);
    return &array->data[array->length - 1];
}

int Array_i8_is_empty(Arena *arena, Array_i8 *array) {
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


