#include "xstring.h"
#define T string_t

void string_init(T *s, size_t length) {
    s->buffer = malloc(length);
    if (!s->buffer) {
        printf("Memory Allocation Failure"); 
    }
    s->length = length;
}

void string_resize(T *s, size_t length) {
    s->buffer = realloc(s, length);
    s->length = length;
}

void string_deinit(T *s) {
    free(s->buffer);
    s->length = 0;
}

void string_insert(T *s, const char *input_string) {
    // printf("%d\n", strlen(input_string));
    size_t input_string_length = strlen(input_string);
    if (input_string_length < s->length) {
        memcpy(s->buffer, input_string,  + 1);
    }
}
#undef T
