#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "list.h"
#include "array.h"
#include "xstring.h"

/*
void ddestroy(void *data) {
    free(data);
}
*/

#define N 256
uint8_t buff[N];

int main() {

    /*
    List *linked_list = NULL;
    if ((linked_list = (List *)malloc(sizeof(List))) == NULL) {
        return -1;
    }
    list_init(linked_list, ddestroy);
    float *data = NULL;
    for (uint32_t i = 0; i < 10; i += 1) {
        data = malloc(sizeof(float));
        *data = 3.141592653589;
        list_ins_next(linked_list, NULL, data);
    }
    ListElmt *ielement = NULL;
    for (uint32_t i = 0; i < 10; i += 1) {
        data = list_head(linked_list)->data;
        printf("%i : %f\n", i, *data);
    }
    list_destroy(linked_list);
    */

    // char array
    array array_of_bytes = {0};
    array_create(&array_of_bytes, sizeof(char), 8);
    char *bcursor = (char *)array_of_bytes.data;

    bcursor[0] = 'a';
    bcursor[1] = 'b';
    bcursor[2] = 'c';
    bcursor[3] = '\0';
    printf("%s\n", (char *)array_of_bytes.data);

    array_destroy(&array_of_bytes);

    // float array
    array array_of_floats = {0};
    array_create(&array_of_floats, sizeof(float), 4);
    float *fhandle = (float *)array_of_floats.data;

    fhandle[0] = 3.141592f;
    fhandle[1] = 1.0f;
    fhandle[2] = 2.0f * 3.141592;
    fhandle[3] = 13.6666f;

    for (int n  = 0; n < array_of_floats.length; n++) {
        printf("%f ", fhandle[n]);
    }
    printf("\n");

    array_resize(&array_of_floats, 6);
    fhandle = (float *)array_of_floats.data;

    for (int n  = 0; n < array_of_floats.length; n++) {
        printf("%f ", fhandle[n]);
    }
    printf("\n");

    float *x = &(float){1.1911f};

    array_insert(&array_of_floats, x);
    array_insert(&array_of_floats, x);
    array_insert(&array_of_floats, x);
    array_insert(&array_of_floats, x);
    fhandle = (float *)array_of_floats.data;

    for (int n  = 0; n < array_of_floats.length; n++) {
        printf("%f ", fhandle[n]);
    }
    printf("\n");

    printf("Array Details: { data: %p, unit: %zu, length: %zu, border: %zu }\n", array_of_floats.data, array_of_floats.unit_size, array_of_floats.length, array_of_floats.border);

    array_destroy(&array_of_floats);

    /*
    printf("string_t program.\n"); 

    buff[0] = 'a';
    buff[1] = 'b';
    buff[2] = 'c';
    buff[3] = '\0';
    printf("%s\n", buff);

    // string memory allocation dynamic
    string_t *s = NULL;
    s = malloc(sizeof(string_t));
    if (!s) {
        printf("Memory Allocation Failure"); 
    }
    // string memory allocation static
    // s = &buff;

    string_init(s, N);

    s->buffer[0] = 'a';
    s->buffer[1] = 'b';
    s->buffer[2] = 'c';
    s->buffer[3] = '\0';
    string_insert(s, "I like to program things in C.");
    printf("%s\n", s->buffer);

    string_insert(s, "I like C.");
    printf("%s\n", s->buffer);

    string_deinit(s);

    // string memory de-allocation
    free(s);
    s = NULL;
    */

    return 0;
}
