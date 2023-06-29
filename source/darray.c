#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "darray.h"

void darray_create(darray *darr, size_t size, size_t count) {
    darr->data = malloc(size * count);
    // darr->data = realloc(NULL, size * count);
    darr->count = count;
    darr->max_cap = count;
    darr->occupied = 0;
}

void darray_insert(darray *darr, void *elem, size_t elem_size) {
    char *p = (char *)darr->data;
    if (darr->occupied < darr->max_cap) {
        //darr->data[darr->occupied] = elem; // void unassignable
        p += darr->occupied * elem_size;
        memcpy(p, elem, elem_size);
        //memcpy(darr->data + darr->occupied, elem, elem_size);
        darr->occupied += 1;
    } else {
        darr->data = realloc(darr->data, darr->max_cap * 2);
        darr->max_cap = darr->max_cap * 2;
        p += darr->occupied;
        memcpy(p, elem, elem_size);
        //darr->data + darr->occupied = elem;
        //memcpy(darr->data+darr->occupied, elem, elem_size);
        darr->occupied += 1;
    }
}

void darray_destroy(darray *darr) {
    free(darr->data);
    darr->max_cap = 0;
    darr->occupied = 0;
}
