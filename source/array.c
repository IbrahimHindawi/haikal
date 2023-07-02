#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "Array.h"

void *array_create(Array *array, size_t unit_size, size_t length) {
    array->unit_size = unit_size;
    array->length = length;
    array->data = malloc(unit_size * length);
    if (array->data == NULL) {
        printf("Memory Allocation Failure!");
        exit(-1);
    }
    // #ifdef DEBUG
    memset(array->data, 0, unit_size * length);
    // #endif
    array->border = length;
    return array->data;
}

// Resize border: Should be a private function.
void *array_resize(Array *array, size_t new_border) {
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
        char *cursor = array->data;
        memset(cursor + (array->unit_size * old_border), 0, array->unit_size * border_difference);
    }
    // #endif
    array->border = new_border;
    return array->data;
}

void *array_append(Array *array, void *elem) {
    // If memory border has been reached -> must reallocate.
    if (array->length == array->border) {
        array->border *= 2;
        array_resize(array, array->border);
    }
    array->length += 1;
    char *cursor = array->data;
    memcpy(cursor + ((array->length - 1) * array->unit_size), elem, array->unit_size);
    return array->data;
    /*
    char *p = (char *)array->data;
    if (array->occupied < array->border) {
        //array->data[array->occupied] = elem; // void unassignable
        p += array->occupied * elem_size;
        memcpy(p, elem, elem_size);
        //memcpy(array->data + array->occupied, elem, elem_size);
        array->occupied += 1;
    } else {
        array->data = realloc(array->data, array->border * 2);
        array->border = array->border * 2;
        p += array->occupied;
        memcpy(p, elem, elem_size);
        //array->data + array->occupied = elem;
        //memcpy(array->data+array->occupied, elem, elem_size);
        array->occupied += 1;
    }
    */
}

void *array_destroy(Array *array) {
    array->border = 0;
    array->length = 0;
    array->unit_size = 0;
    free(array->data);
    return array->data;
}

/*
void array_resize(Array *array, size_t new_length) {
    size_t old_length = array->length;
    array->length = new_length;
    array->data = realloc(array->data, array->unit_size * array->length);
    if (array->data == NULL) {
        printf("Memory Reallocation Failure!");
        exit(-1);
    }
    // #ifdef DEBUG
    if (new_length > old_length) {
        size_t length_difference = new_length - old_length;
        char *cursor = array->data;
        memset(cursor + (array->unit_size * old_length), 0, array->unit_size * length_difference);
    }
    // #endif
    array->border = new_length;
}
*/