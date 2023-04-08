#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T stack_t
static int count = 0;

typedef struct T {
    char *stack_mem;
    char *cursor;
    size_t stack_size;
    size_t element_size;
} T;

void stack_init(T *stack, size_t stack_size, size_t element_size);

void stack_deinit(T *stack);

void stack_push(T *stack, void *item);

void stack_pop(T *stack);

void *stack_peek(T *stack);

#undef T
