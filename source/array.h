#ifndef DARRAY_H
#define DARRAY_H

typedef struct Array {
    void *data;
    size_t unit_size;
    size_t length;
    size_t border;
} Array;

void *array_create(Array *array, size_t unit_size, size_t length);

void *array_resize(Array *array, size_t new_length);

void *array_append(Array *array, void *elem);

void *array_destroy(Array *array);

#define array_print(array, handle, fmt) { \
    printf("Array Metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n", (array).data, (array).unit_size, (array).length, (array).border); \
    printf("Array Components: {"); \
    for (int n  = 0; n < (array).length; n++) { \
        printf( fmt ", ", handle[n]); \
    } \
    printf("\b\b}\n\n"); \
} \

#define array_print_struct(array, handle, fmt) { \
    printf("Array Metadata: { data: %p, unit_size: %zu, length: %zu, border: %zu }\n", (array).data, (array).unit_size, (array).length, (array).border); \
    printf("Array Components: {"); \
    for (int n  = 0; n < (array).length; n++) { \
        printf( fmt ); \
    } \
    printf("\b\b}\n\n"); \
} \

#endif
