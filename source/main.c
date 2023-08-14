#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "list.h"
#include "hkArray.h"
#include "xstring.h"

// check hackerrank videos

typedef struct vec3 {
    float x, y, z;
} vec3;
#define vec3ffmt(handle) "<%f, %f, %f>" " ", (handle)[n].x, (handle)[n].y, (handle)[n].z


#define N 256
uint8_t buff[N];

int main() {

    // char array
    hkArray bytes = hkArrayCreate(sizeof(char), 4);
    char *bhandle = bytes.data;
    hkArrayPrint(bytes, bhandle, "%c");

    bhandle[0] = 'a';
    bhandle[1] = 'b';
    bhandle[2] = 'c';
    bhandle[3] = '\0';
    printf("%s\n", (char *)bytes.data);
    hkArrayPrint(bytes, bhandle, "%c");

    bhandle = hkArrayDestroy(&bytes);

    // float array
    hkArray floats = hkArrayCreate(sizeof(float), 4);
    float *fhandle = floats.data;
    hkArrayPrint(
    floats, fhandle, "%f");

    fhandle[0] = 3.141592f;
    fhandle[1] = 1.0f;
    fhandle[2] = 2.0f * 3.141592;
    fhandle[3] = 13.6666f;
    hkArrayPrint(floats, fhandle, "%f");


    fhandle = hkArrayResize(&floats, 6);
    hkArrayPrint(floats, fhandle, "%f");

    {
        float *x = &(float){1.1911f};
        fhandle = hkArrayAppend(&floats, x);
        fhandle = hkArrayAppend(&floats, x);
        fhandle = hkArrayAppend(&floats, x);
        fhandle = hkArrayAppend(&floats, x);
        fhandle = hkArrayAppend(&floats, x);
        fhandle = hkArrayAppend(&floats, x);
        fhandle = hkArrayAppend(&floats, x);
    }
    hkArrayPrint(floats, fhandle, "%f");

    fhandle = hkArrayDestroy(&floats);

    hkArray vectors = hkArrayCreate(sizeof(vec3), 3);
    vec3 *vhandle = vectors.data;
    if (hkArrayIsEmpty(&vectors) == 1) {
        printf("hkArray is empty.\n");
    } else {
        printf("hkArray is not empty.\n");
    }

    vhandle[0] = (vec3){1.0f, 0.0f, 0.0f};
    vhandle[1] = (vec3){0.0f, 1.0f, 0.0f};
    vhandle[2] = (vec3){0.0f, 0.0f, 1.0f};
    hkArrayPrintStruct(vectors, vhandle, vec3ffmt(vhandle));

    // int n = 0; hkArrayPrint_(&vectors, vectors.length, "<%f, %f, %f>" " ", vhandle[n].x, vhandle[n].y, vhandle[n].z);

    vhandle = hkArrayDestroy(&vectors);

    hkArray ints = hkArrayCreate(sizeof(uint64_t), 0);
    uint64_t *ints_handle = ints.data;
    hkArrayPrint(ints, ints_handle, "%lld");
    {
        uint64_t *n = &(uint64_t){1};
        ints_handle = hkArrayAppend(&ints, n);
        ints_handle = hkArrayAppend(&ints, n);
        ints_handle = hkArrayAppend(&ints, n);
        ints_handle = hkArrayAppend(&ints, n);
        ints_handle = hkArrayAppend(&ints, n);
        ints_handle = hkArrayAppend(&ints, n);
    }
    hkArrayPrint(ints, ints_handle, "%lld");

    hkArray ints2  = hkArrayCreate(sizeof(uint64_t), 0);
    uint64_t *ints2_handle = ints2.data;
    hkArrayPrint(ints2, ints2_handle, "%lld");
    {
        uint64_t *n = &(uint64_t){-1};
        ints2_handle = hkArrayAppend(&ints2, n);
        ints2_handle = hkArrayAppend(&ints2, n);
        ints2_handle = hkArrayAppend(&ints2, n);
        ints2_handle = hkArrayAppend(&ints2, n);
        ints2_handle = hkArrayAppend(&ints2, n);
        ints2_handle = hkArrayAppend(&ints2, n);
    }
    hkArrayPrint(ints2, ints2_handle, "%lld");

    assert(ints.length == ints2.length);

    hkArray intsresult = hkArrayCreate(sizeof(uint64_t), ints.length);
    uint64_t *intsresult_handle = intsresult.data;
    for (int x = 0; x < ints.length; x++) {
        intsresult_handle[x] = ints_handle[x] + ints2_handle[x];
    }
    hkArrayPrint(intsresult, intsresult_handle, "%lld");

    // printf("VarArgs Procedure Execute...\n");
    // hkArrayPrint__(intsresult, intsresult.length, " i:%d, y:%lld ", intsresult_handle);

    ints_handle = hkArrayDestroy(&ints);
    ints2_handle = hkArrayDestroy(&ints2);
    intsresult_handle = hkArrayDestroy(&intsresult);

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

    return 0;
}
