#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "list.h"
#include "Array.h"
#include "xstring.h"

/*
void ddestroy(void *data) {
    free(data);
}
*/

typedef struct vec3f {
    float x, y, z;
} vec3f;
#define vec3ffmt(handle) "<%f, %f, %f>" ", ", (handle)[n].x, (handle)[n].y, (handle)[n].z

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
    Array bytes = {0};
    char *bhandle = array_create(&bytes, sizeof(char), 4);
    array_print(bytes, bhandle, "%c");

    bhandle[0] = 'a';
    bhandle[1] = 'b';
    bhandle[2] = 'c';
    bhandle[3] = '\0';
    printf("%s\n", (char *)bytes.data);
    array_print(bytes, bhandle, "%c");

    bhandle = array_destroy(&bytes);

    // float array
    Array floats = {0};
    float *fhandle = array_create(&floats, sizeof(float), 4);
    array_print(floats, fhandle, "%f");

    fhandle[0] = 3.141592f;
    fhandle[1] = 1.0f;
    fhandle[2] = 2.0f * 3.141592;
    fhandle[3] = 13.6666f;
    array_print(floats, fhandle, "%f");


    fhandle = array_resize(&floats, 6);
    array_print(floats, fhandle, "%f");

    {
        float *x = &(float){1.1911f};
        fhandle = array_append(&floats, x);
        fhandle = array_append(&floats, x);
        fhandle = array_append(&floats, x);
        fhandle = array_append(&floats, x);
        fhandle = array_append(&floats, x);
        fhandle = array_append(&floats, x);
        fhandle = array_append(&floats, x);
    }
    array_print(floats, fhandle, "%f");

    fhandle = array_destroy(&floats);

    Array vectors = {0};
    vec3f *vhandle = array_create(&vectors, sizeof(vec3f), 3);
    if (array_is_empty(&vectors) == 1) {
        printf("Array is empty.\n");
    } else {
        printf("Array is not empty.\n");
    }

    vhandle[0] = (vec3f){1.0f, 0.0f, 0.0f};
    vhandle[1] = (vec3f){0.0f, 1.0f, 0.0f};
    vhandle[2] = (vec3f){0.0f, 0.0f, 1.0f};
    array_print_struct(vectors, vhandle, vec3ffmt(vhandle));

    vhandle = array_destroy(&vectors);

    Array ints = {0};
    uint64_t *ints_handle = array_create(&ints, sizeof(uint64_t), 0);
    array_print(ints, ints_handle, "%lld");
    {
        uint64_t *n = &(uint64_t){18183365};
        ints_handle = array_append(&ints, n);
        ints_handle = array_append(&ints, n);
        ints_handle = array_append(&ints, n);
        ints_handle = array_append(&ints, n);
        ints_handle = array_append(&ints, n);
        ints_handle = array_append(&ints, n);
    }
    array_print(ints, ints_handle, "%lld");

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
