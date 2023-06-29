#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "list.h"
#include "darray.h"
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
    darray da = {0};
    darray_create(&da, sizeof(char), 8);

    char *cursor = (char *)da.data;
    cursor[0] = 'a';
    cursor[1] = 'b';
    cursor[2] = 'c';
    cursor[3] = '\0';
    printf("%s\n", (char *)da.data);

    darray_destroy(&da);

    // float array
    darray daf = {0};
    darray_create(&daf, sizeof(float), 8);

    float *fcursor = (float *)daf.data;
    fcursor[0] = 3.141592f;
    fcursor[1] = 1.0f;
    fcursor[2] = 2.0f * 3.141592;
    fcursor[3] = 1.0f;

    for (int n  = 0; n < daf.count; n++) {
        printf("%f ", fcursor[n]);
    }

    darray_destroy(&daf);


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
