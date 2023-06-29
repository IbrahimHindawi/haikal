#ifndef DARRAY_H
#define DARRAY_H

typedef struct darray_ {
    void *data;
    size_t count;
    size_t max_cap;
    size_t occupied;
} darray;

void darray_create(darray *darr, size_t size, size_t count);

void darray_insert(darray *darr, void *elem, size_t elem_size);

void darray_destroy(darray *darr);

#endif
