#ifndef DARRAY_H
#define DARRAY_H

typedef struct darray_ {
    size_t capacity;
    size_t occupied;
    void *data;
} darray;

void darray_init(darray *darr, size_t elem_size);

void darray_insert(darray *darr, void *elem, size_t elem_size);

#endif
