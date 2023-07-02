#ifndef DARRAY_H
#define DARRAY_H

typedef struct Array {
    void *data;
    size_t unit_size;
    size_t length;
    size_t border;
} Array;

void *array_create(Array *arr, size_t unit_size, size_t length);

void *array_resize(Array *arr, size_t new_length);

void *array_insert(Array *arr, void *elem);

void *array_destroy(Array *arr);

#endif
