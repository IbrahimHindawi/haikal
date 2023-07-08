#ifndef string_h
#define string_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T string_t

typedef struct T {
    uint8_t *buffer;
    size_t length;
} T;

void string_init(T *s, size_t length);

void string_resize(T *s, size_t length);

void string_deinit(T *s);

void string_insert(T *s, const char *input_string);

#undef T
#endif
