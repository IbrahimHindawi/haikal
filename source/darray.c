#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "darray.h"

void darray_init(darray *darr, size_t elem_size) {
    darr->data = malloc(elem_size * 2);
    darr->capacity = 2;
    darr->occupied = 0;
}

void darray_insert(darray *darr, void *elem, size_t elem_size) {
    char *p = (char *)darr->data;
    if (darr->occupied < darr->capacity) {
        //darr->data[darr->occupied] = elem; // void unassignable
        p += darr->occupied * elem_size;
        memcpy(p, elem, elem_size);
        //memcpy(darr->data + darr->occupied, elem, elem_size);
        darr->occupied += 1;
    } else {
        darr->data = realloc(darr->data, darr->capacity * 2);
        darr->capacity = darr->capacity * 2;
        p += darr->occupied;
        memcpy(p, elem, elem_size);
        //darr->data + darr->occupied = elem;
        //memcpy(darr->data+darr->occupied, elem, elem_size);
        darr->occupied += 1;
    }
}
