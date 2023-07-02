#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "Array.h"

void *array_create(Array *arr, size_t unit_size, size_t length) {
    arr->unit_size = unit_size;
    arr->length = length;
    arr->data = malloc(unit_size * length);
    if (arr->data == NULL) {
        printf("Memory Allocation Failure!");
        exit(-1);
    }
    // #ifdef DEBUG
    memset(arr->data, 0, unit_size * length);
    // #endif
    arr->border = length;
    return arr->data;
}

// Resize border: Should be a private function.
void *array_resize(Array *arr, size_t new_border) {
    size_t old_border = arr->border;
    arr->border = new_border;
    arr->data = realloc(arr->data, arr->unit_size * arr->border);
    if (arr->data == NULL) {
        printf("Memory Reallocation Failure!");
        exit(-1);
    }
    // #ifdef DEBUG
    if (new_border > old_border) {
        size_t border_difference = new_border - old_border;
        char *cursor = arr->data;
        memset(cursor + (arr->unit_size * old_border), 0, arr->unit_size * border_difference);
    }
    // #endif
    arr->border = new_border;
    return arr->data;
}

void *array_insert(Array *arr, void *elem) {
    // If memory border has been reached -> must reallocate.
    if (arr->length == arr->border) {
        arr->border *= 2;
        array_resize(arr, arr->border);
    }
    arr->length += 1;
    char *cursor = arr->data;
    memcpy(cursor + ((arr->length - 1) * arr->unit_size), elem, arr->unit_size);
    return arr->data;
    /*
    char *p = (char *)arr->data;
    if (arr->occupied < arr->border) {
        //arr->data[arr->occupied] = elem; // void unassignable
        p += arr->occupied * elem_size;
        memcpy(p, elem, elem_size);
        //memcpy(arr->data + arr->occupied, elem, elem_size);
        arr->occupied += 1;
    } else {
        arr->data = realloc(arr->data, arr->border * 2);
        arr->border = arr->border * 2;
        p += arr->occupied;
        memcpy(p, elem, elem_size);
        //arr->data + arr->occupied = elem;
        //memcpy(arr->data+arr->occupied, elem, elem_size);
        arr->occupied += 1;
    }
    */
}

void *array_destroy(Array *arr) {
    arr->border = 0;
    arr->length = 0;
    arr->unit_size = 0;
    free(arr->data);
    return arr->data;
}

/*
void array_resize(Array *arr, size_t new_length) {
    size_t old_length = arr->length;
    arr->length = new_length;
    arr->data = realloc(arr->data, arr->unit_size * arr->length);
    if (arr->data == NULL) {
        printf("Memory Reallocation Failure!");
        exit(-1);
    }
    // #ifdef DEBUG
    if (new_length > old_length) {
        size_t length_difference = new_length - old_length;
        char *cursor = arr->data;
        memset(cursor + (arr->unit_size * old_length), 0, arr->unit_size * length_difference);
    }
    // #endif
    arr->border = new_length;
}
*/
