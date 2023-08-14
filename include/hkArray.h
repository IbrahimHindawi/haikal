#ifndef DARRAY_H
#define DARRAY_H

typedef struct hkArray {
    void *data;
    size_t unit_size;
    size_t length;
    size_t border;
} hkArray;

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

hkArray hkArrayCreate(size_t unit_size, size_t length);

void hkArrayDestroy(hkArray *array);

void *hkArrayResize(hkArray *array, size_t new_length);

void *hkArrayAppend(hkArray *array, void *elem);

int hkArrayIsEmpty(hkArray *array);


#define hkArrayPrint(array, handle, fmt) { \
    printf("metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n", \
            (array).data, (array).unit_size, (array).length, (array).border); \
    printf("components: { "); \
    for (int n  = 0; n < (array).length; n++) { \
        printf( fmt " ", handle[n]); \
    } \
    printf("}\n\n"); \
} \

#define hkArrayPrintStruct(array, handle, fmt) { \
    printf("metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n", \
            (array).data, (array).unit_size, (array).length, (array).border); \
    printf("components: { "); \
    for (int n  = 0; n < (array).length; n++) { \
        printf( fmt ); \
    } \
    printf("\b\b}\n\n"); \
} \

#endif

// void array_print_(hkArray *array, int iterator, const char *fmt, ...);
