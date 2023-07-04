#ifndef DARRAY_H
#define DARRAY_H

typedef struct Array {
    void *data;
    size_t unit_size;
    size_t length;
    size_t border;
} Array;

/*
 * create: create a newly allocated data
 * destroy: destroy existing data
 * append: append item at the end of data, possibly double memory
 * prepend: prepend item at the beginning of data
 * insert: insert item in place of item
 * remove: remove item from data, shrink memory
 * resize: resize data to desired size
 * consolidate: fit memory border into length
 * slice: create a newly allocated data from existing data slice
 * is_empty: check if array has 0 items
 * print: display data to console
*/

void *array_create(Array *array, size_t unit_size, size_t length);

void *array_destroy(Array *array);

void *array_resize(Array *array, size_t new_length);

void *array_append(Array *array, void *elem);

int array_is_empty(Array *array);


#define array_print(array, handle, fmt) { \
    printf("metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n", \
            (array).data, (array).unit_size, (array).length, (array).border); \
    printf("components: { "); \
    for (int n  = 0; n < (array).length; n++) { \
        printf( fmt " ", handle[n]); \
    } \
    printf("}\n\n"); \
} \

#define array_print_struct(array, handle, fmt) { \
    printf("metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n", \
            (array).data, (array).unit_size, (array).length, (array).border); \
    printf("components: { "); \
    for (int n  = 0; n < (array).length; n++) { \
        printf( fmt ); \
    } \
    printf("\b\b}\n\n"); \
} \

#endif

// void array_print_(Array *array, int iterator, const char *fmt, ...);
