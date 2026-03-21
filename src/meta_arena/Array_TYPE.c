#include <core.h>

#include "Array_TYPE.h"

Array_TYPE Array_TYPE_reserve(memops_arena *arena, u64 length) {
    Array_TYPE array = {0};
    if (length == 0) {
        return array;
    }
    array.data = memops_arena_push_array(arena, TYPE, length);
    if (array.data == NULL) {
        printf("haikal::Memory Allocation Failure!\n");
        exit(-1);
    }
    memset(array.data, 0, sizeof(TYPE) * length);
    array.length = length;
    return array;
}

void Array_TYPE_destroy(memops_arena *arena, Array_TYPE *array) {
    (void)arena;
    array->length = 0;
    array->data = NULL;
}

TYPE *Array_TYPE_at(Array_TYPE *array, u64 index) {
    if (array == NULL || index >= array->length) {
        printf("haikal::Array Index Out of Bounds!\n");
        exit(-1);
    }
    return &array->data[index];
}

int Array_TYPE_is_empty(memops_arena *arena, Array_TYPE *array) {
    (void)arena;
    return array->length == 0 ? 1 : 0;
}

