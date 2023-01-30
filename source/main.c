#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "list.h"
#include "darray.h"

void ddestroy(void *data) {
    free(data);
}

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

    darray da = {0};
    darray_init(&da, sizeof(float));
    float x = 1.0f;
    float y = 3.141592653589f;
    float z = 0.0f;
    darray_insert(&da, &x, sizeof(float));
    darray_insert(&da, &y, sizeof(float));
    darray_insert(&da, &z, sizeof(float));
    printf("%zu\n", da.capacity);
    printf("%zu\n", da.occupied);

    return 0;
}
