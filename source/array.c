#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "array.h"

void array_create(array *arr, size_t unit_size, size_t length) {
    arr->unit_size = unit_size;
    arr->length = length;
    arr->data = malloc(unit_size * length);
    if (arr->data == NULL) {
        printf("Memory Allocation Failure!");
        exit(-1);
    }
    memset(arr->data, 0, unit_size * length);
    // arr->data = realloc(NULL, size * length);
    arr->max_cap = length;
}

void array_resize(array *arr, size_t new_length) {
    size_t old_length = arr->length;
    arr->length = new_length;
    realloc(arr->data, arr->unit_size * arr->length);
    if (arr->data == NULL) {
        printf("Memory Reallocation Failure!");
        exit(-1);
    }
    if (new_length > old_length) {
        size_t length_difference = new_length - old_length;
        char *cursor = arr->data;
        memset(cursor + (arr->unit_size * old_length), 0, arr->unit_size * length_difference);
    }
}

void array_insert(array *arr, void *elem, size_t elem_size) {
    /*
    char *p = (char *)arr->data;
    if (arr->occupied < arr->max_cap) {
        //arr->data[arr->occupied] = elem; // void unassignable
        p += arr->occupied * elem_size;
        memcpy(p, elem, elem_size);
        //memcpy(arr->data + arr->occupied, elem, elem_size);
        arr->occupied += 1;
    } else {
        arr->data = realloc(arr->data, arr->max_cap * 2);
        arr->max_cap = arr->max_cap * 2;
        p += arr->occupied;
        memcpy(p, elem, elem_size);
        //arr->data + arr->occupied = elem;
        //memcpy(arr->data+arr->occupied, elem, elem_size);
        arr->occupied += 1;
    }
    */
}

void array_destroy(array *arr) {
    free(arr->data);
    arr->max_cap = 0;
    arr->length = 0;
    arr->unit_size = 0;
}
