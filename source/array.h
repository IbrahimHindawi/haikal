#ifndef DARRAY_H
#define DARRAY_H

typedef struct array {
    void *data;
    size_t unit_size;
    size_t length;
    size_t max_cap;
} array;

void array_create(array *arr, size_t unit_size, size_t length);

void array_resize(array *arr, size_t new_length);

void array_insert(array *arr, void *elem, size_t elem_size);

void array_destroy(array *arr);

#endif
