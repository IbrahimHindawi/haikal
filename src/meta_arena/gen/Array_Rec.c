#include <core.h>

#include "Array_Rec.h"

Array_Rec Array_Rec_reserve(Arena *arena, u64 length) {
    Array_Rec array = {0};
    if (length == 0) {
        return array;
    }
    array.length = length;
    array.data = arenaPushArray(arena, Rec, length);
    // array.data = malloc(sizeof(Rec) * length);
    if (array.data == NULL) {
        printf("haikal::Memory Allocation Failure!\n");
        exit(-1);
    }
    // #ifdef DEBUG
    memset(array.data, 0, sizeof(Rec) * length);
    // #endif
    array.border = length;
    return array;
}

void Array_Rec_destroy(Arena *arena, Array_Rec *array) {
    array->border = 0;
    array->length = 0;
    // free(array->data);
    array->data = NULL;
}

Rec *Array_Rec_resize(Arena *arena, Array_Rec *array) {
    u64 old_border = array->border;
    array->border *= 2;
    array->data = arenaRealloc(arena, Rec, array->border, array->data, old_border);
    // array->data = realloc(arena, array->data, sizeof(Rec) * array->border);
    if (array->data == NULL) {
        printf("haikal::Memory Reallocation Failure!\n");
        exit(-1);
    }
    // #ifdef DEBUG
    if (array->border > old_border) {
        u64 border_difference = array->border - old_border;
        char *cursor = (char *)array->data;
        memset(cursor + (sizeof(Rec) * old_border), 0, sizeof(Rec) * border_difference);
    }
    // #endif
    return array->data;
}

Rec *Array_Rec_append(Arena *arena, Array_Rec *array, Rec elem) {
    if (array->length == 0 && array->border == 0) { 
        array->length += 1;
        array->border += 1;
        array->data = arenaPushArray(arena, Rec, array->border);
        // array->data = realloc(NULL, sizeof(Rec) * array->border);
        array->data[array->length - 1] = elem;
        // char *cursor = (char *)array->data;
        // memcpy(cursor, elem, array->unit_size);
        return array->data;
    }
    if (array->length == array->border ) {
        array->border *= 2;
        // TODO: check if it zeroes out properly
        Array_Rec_resize(arena, array);
    }
    array->length += 1;
    array->data[array->length - 1] = elem;
    // char *cursor = (char *)array->data;
    // memcpy(cursor + ((array->length - 1) * array->unit_size), elem, array->unit_size);
    return &array->data[array->length - 1];
}

int Array_Rec_is_empty(Arena *arena, Array_Rec *array) {
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


